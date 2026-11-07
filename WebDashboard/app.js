/* ============================================================
 * app.js — STM32 Industrial Monitor Dashboard
 * Polls /api/all every 1s, /api/system every 5s
 * Updates Chart.js gauges, history charts, status badges
 * ============================================================ */

'use strict';

/* ── Config ────────────────────────────────────────────────── */
const API_ALL    = '/api/all';
const API_SYS    = '/api/system';
const API_NET    = '/GetNetInfo';
const POLL_MS    = 1000;
const SYS_MS     = 5000;
const HISTORY_LEN = 60;   /* data points = 60 seconds */

/* ── Sensor definitions ────────────────────────────────────── */
const SENSORS = [
  { id:'temp',  key:'temperature', label:'Temperature', unit:'°C',  min:-50,  max:150,  color:'#ff6b6b', alertKey:'temp_alert',    okKey:'temp_ok'    },
  { id:'pres',  key:'pressure',    label:'Pressure',    unit:'hPa', min:800,  max:1200, color:'#4ecdc4', alertKey:null,            okKey:'pressure_ok' },
  { id:'lvl',   key:'level',       label:'Level',       unit:'cm',  min:0,    max:200,  color:'#45b7d1', alertKey:null,            okKey:'level_ok'   },
  { id:'rpm',   key:'rpm',         label:'Speed',       unit:'RPM', min:0,    max:3000, color:'#96ceb4', alertKey:null,            okKey:'rpm_ok'     },
  { id:'curr',  key:'current',     label:'Current',     unit:'A',   min:0,    max:20,   color:'#ffd93d', alertKey:'current_alert', okKey:'current_ok' },
  { id:'gas',   key:'gas',         label:'Gas',         unit:'%',   min:0,    max:100,  color:'#ff6348', alertKey:'gas_alert',     okKey:'gas_ok'     },
  { id:'light', key:'light_lux',   label:'Light',       unit:'lux', min:0,    max:1200, color:'#a8dadc', alertKey:null,            okKey:'light_ok'   },
];

/* ── State ─────────────────────────────────────────────────── */
let gaugeCharts    = {};
let histTempPres   = null;
let histRpmCurr    = null;
let histData       = {
  temp:     Array(HISTORY_LEN).fill(null),
  pressure: Array(HISTORY_LEN).fill(null),
  rpm:      Array(HISTORY_LEN).fill(null),
  current:  Array(HISTORY_LEN).fill(null),
};
let lastData      = null;
let pollTimer     = null;
let sysTimer      = null;
let errorCount    = 0;
let startTime     = Date.now();

/* ── Chart.js global defaults ──────────────────────────────── */
Chart.defaults.color           = 'rgba(232,237,245,0.5)';
Chart.defaults.font.family     = "'Inter', sans-serif";
Chart.defaults.font.size       = 11;

/* ============================================================
 * GAUGE CHART FACTORY
 * Uses a 270-degree doughnut (speedometer style)
 * ============================================================ */
function createGauge(canvasId, sensor) {
  const ctx = document.getElementById(canvasId);
  if (!ctx) return null;

  return new Chart(ctx, {
    type: 'doughnut',
    data: {
      datasets: [{
        data: [0, sensor.max - sensor.min],
        backgroundColor: [sensor.color, 'rgba(255,255,255,0.05)'],
        borderWidth: 0,
        hoverBackgroundColor: [sensor.color, 'rgba(255,255,255,0.05)'],
        borderRadius: 4,
      }]
    },
    options: {
      circumference: 270,
      rotation: -135,
      cutout: '72%',
      responsive: false,
      maintainAspectRatio: false,
      plugins: {
        legend:  { display: false },
        tooltip: { enabled: false },
      },
      animation: {
        duration: 600,
        easing: 'easeInOutCubic',
      }
    }
  });
}

/* ============================================================
 * HISTORY LINE CHART FACTORY
 * Dual Y-axis for two different unit ranges
 * ============================================================ */
function createHistoryChart(canvasId, datasets) {
  const ctx = document.getElementById(canvasId);
  if (!ctx) return null;

  const labels = Array.from({ length: HISTORY_LEN }, (_, i) => {
    const s = HISTORY_LEN - 1 - i;
    return s === 0 ? 'now' : `-${s}s`;
  });

  return new Chart(ctx, {
    type: 'line',
    data: {
      labels,
      datasets: datasets.map((ds, i) => ({
        label: ds.label,
        data: Array(HISTORY_LEN).fill(null),
        borderColor: ds.color,
        backgroundColor: ds.color + '18',
        fill: true,
        tension: 0.4,
        pointRadius: 0,
        pointHoverRadius: 4,
        borderWidth: 2,
        yAxisID: i === 0 ? 'y1' : 'y2',
        spanGaps: true,
      }))
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: { duration: 0 },
      interaction: { mode: 'index', intersect: false },
      scales: {
        x: {
          grid:  { color: 'rgba(255,255,255,0.04)', drawTicks: false },
          ticks: { maxTicksLimit: 7, color: 'rgba(232,237,245,0.3)', font: { size: 10 } },
        },
        y1: {
          position: 'left',
          grid:  { color: 'rgba(255,255,255,0.04)' },
          ticks: { color: datasets[0].color + 'aa', font: { size: 10 } },
          title: { display: true, text: datasets[0].unit, color: datasets[0].color + '99', font: { size: 10 } },
        },
        y2: {
          position: 'right',
          grid:  { display: false },
          ticks: { color: datasets[1].color + 'aa', font: { size: 10 } },
          title: { display: true, text: datasets[1].unit, color: datasets[1].color + '99', font: { size: 10 } },
        },
      },
      plugins: {
        legend: {
          labels: {
            color: 'rgba(232,237,245,0.6)',
            boxWidth: 12,
            font: { size: 11 },
            usePointStyle: true,
            pointStyle: 'line',
          }
        },
        tooltip: {
          backgroundColor: 'rgba(13,21,38,0.95)',
          borderColor: 'rgba(255,255,255,0.1)',
          borderWidth: 1,
          padding: 10,
        }
      }
    }
  });
}

/* ============================================================
 * GAUGE UPDATE
 * ============================================================ */
function updateGauge(sensor, value) {
  const chart = gaugeCharts[sensor.id];
  if (!chart) return;

  const clamped = Math.min(Math.max(value, sensor.min), sensor.max);
  const filled  = clamped - sensor.min;
  const empty   = sensor.max - sensor.min - filled;

  chart.data.datasets[0].data            = [filled, Math.max(empty, 0)];
  chart.data.datasets[0].backgroundColor = [sensor.color, 'rgba(255,255,255,0.05)'];
  chart.update('active');
}

/* ============================================================
 * VALUE DISPLAY UPDATE
 * ============================================================ */
function updateDisplay(sensor, value, data) {
  /* Format value */
  const el = document.getElementById(`val-${sensor.id}`);
  if (!el) return;

  let formatted;
  if (sensor.id === 'rpm')   formatted = Math.round(value).toString();
  else if (sensor.id === 'pres') formatted = value.toFixed(1);
  else if (sensor.id === 'curr') formatted = value.toFixed(2);
  else                       formatted = value.toFixed(1);

  if (el.textContent !== formatted) {
    el.textContent = formatted;
    el.classList.remove('flash');
    void el.offsetWidth; /* force reflow */
    el.classList.add('flash');
  }

  /* Range bar (0–100%) */
  const bar = document.getElementById(`bar-${sensor.id}`);
  if (bar) {
    const pct = ((value - sensor.min) / (sensor.max - sensor.min)) * 100;
    bar.style.width = Math.min(Math.max(pct, 0), 100) + '%';
  }

  /* Status badge */
  const badge = document.getElementById(`status-${sensor.id}`);
  if (badge) {
    const ok    = data[sensor.okKey];
    const alert = sensor.alertKey ? data[sensor.alertKey] : false;
    if (!ok) {
      badge.textContent = 'ERR';
      badge.className   = 'status-badge error';
    } else if (alert) {
      badge.textContent = 'ALERT';
      badge.className   = 'status-badge warn';
    } else {
      badge.textContent = 'OK';
      badge.className   = 'status-badge ok';
    }
  }

  /* Card alert highlight */
  const card = document.getElementById(`card-${sensor.id}`);
  if (card) {
    if (sensor.alertKey && data[sensor.alertKey]) {
      card.classList.add('alert');
    } else {
      card.classList.remove('alert');
    }
  }
}

/* ============================================================
 * HISTORY PUSH
 * ============================================================ */
function pushHistory(key, value) {
  histData[key].push(value);
  if (histData[key].length > HISTORY_LEN) histData[key].shift();
}

function updateHistoryCharts() {
  if (histTempPres) {
    histTempPres.data.datasets[0].data = [...histData.temp];
    histTempPres.data.datasets[1].data = [...histData.pressure];
    histTempPres.update('none');
  }
  if (histRpmCurr) {
    histRpmCurr.data.datasets[0].data = [...histData.rpm];
    histRpmCurr.data.datasets[1].data = [...histData.current];
    histRpmCurr.update('none');
  }
}

/* ============================================================
 * ALERT BANNER
 * ============================================================ */
function checkAlerts(data) {
  const alerts = [];
  if (data.gas_alert)     alerts.push(`⚠ GAS: ${data.gas.toFixed(1)}% (threshold: 75%)`);
  if (data.temp_alert)    alerts.push(`⚠ TEMP: ${data.temperature.toFixed(1)}°C (threshold: 80°C)`);
  if (data.current_alert) alerts.push(`⚠ CURRENT: ${data.current.toFixed(2)}A (threshold: 8A)`);

  const banner = document.getElementById('alert-banner');
  const text   = document.getElementById('alert-text');
  if (alerts.length > 0) {
    text.textContent = alerts.join('  ·  ');
    banner.classList.add('visible');
  } else {
    banner.classList.remove('visible');
  }
}

/* ============================================================
 * UPTIME FORMATTER
 * ============================================================ */
function formatUptime(seconds) {
  const h = Math.floor(seconds / 3600);
  const m = Math.floor((seconds % 3600) / 60);
  const s = seconds % 60;
  return `${String(h).padStart(2,'0')} : ${String(m).padStart(2,'0')} : ${String(s).padStart(2,'0')}`;
}

/* ============================================================
 * MAIN DATA FETCH — /api/all
 * ============================================================ */
async function fetchAll() {
  try {
    const resp = await fetch(API_ALL, { cache: 'no-store', signal: AbortSignal.timeout(3000) });
    if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
    const data = await resp.json();
    lastData   = data;
    errorCount = 0;

    /* Connection status */
    setConnected(true);

    /* Sim badge visibility */
    const simBadge = document.getElementById('sim-badge');
    if (simBadge) simBadge.classList.toggle('hidden', !data.sim_mode);

    /* Uptime */
    const uptimeEl = document.getElementById('uptime');
    if (uptimeEl && data.uptime != null) uptimeEl.textContent = formatUptime(data.uptime);

    /* Sys mode label */
    const modeEl = document.getElementById('sys-mode');
    if (modeEl) {
      modeEl.textContent = data.sim_mode ? 'SIMULATION' : 'HARDWARE';
      modeEl.style.color = data.sim_mode ? '#ffd93d' : '#00e676';
    }

    /* Update all 7 sensors */
    SENSORS.forEach(sensor => {
      const value = data[sensor.key];
      if (value == null) return;
      updateGauge(sensor, value);
      updateDisplay(sensor, value, data);
    });

    /* Push to history */
    pushHistory('temp',     data.temperature);
    pushHistory('pressure', data.pressure);
    pushHistory('rpm',      data.rpm);
    pushHistory('current',  data.current);
    updateHistoryCharts();

    /* Alerts */
    checkAlerts(data);

  } catch (err) {
    errorCount++;
    if (errorCount >= 3) setConnected(false);
  }
}

/* ============================================================
 * SYSTEM STATS FETCH — /api/system
 * ============================================================ */
async function fetchSystem() {
  try {
    const resp = await fetch(API_SYS, { cache: 'no-store', signal: AbortSignal.timeout(3000) });
    if (!resp.ok) return;
    const d = await resp.json();

    const set = (id, val) => { const el = document.getElementById(id); if (el && val != null) el.textContent = val.toLocaleString(); };
    set('sys-res',   d.tx_resumptions);
    set('sys-sus',   d.tx_suspensions);
    set('sys-acq',   d.acq_cycles);
    set('sys-idle',  d.tx_idle_returns);
    set('sys-nidle', d.tx_non_idle_returns);
  } catch (_) { /* silent */ }
}

/* ============================================================
 * NETWORK INFO — /GetNetInfo (existing route)
 * ============================================================ */
async function fetchNetInfo() {
  try {
    const resp = await fetch(API_NET, { cache: 'no-store', signal: AbortSignal.timeout(3000) });
    if (!resp.ok) return;
    const text = await resp.text();
    /* Format: "192.168.1.50,80" */
    const parts = text.trim().split(',');
    const ipEl  = document.getElementById('device-ip');
    if (ipEl && parts[0]) ipEl.textContent = parts[0];
  } catch (_) { /* silent — IP available on the LAN bar */ }
}

/* ============================================================
 * CONNECTION STATE
 * ============================================================ */
function setConnected(online) {
  const dot   = document.getElementById('conn-dot');
  const label = document.getElementById('conn-label');
  if (dot)   { dot.className   = 'conn-dot ' + (online ? 'online' : 'offline'); }
  if (label) { label.textContent = online ? 'Live'  : 'Reconnecting…'; }
}

/* ============================================================
 * INIT — Build all charts then start polling
 * ============================================================ */
function init() {
  /* Create 7 gauges */
  SENSORS.forEach(sensor => {
    gaugeCharts[sensor.id] = createGauge(`gauge-${sensor.id}`, sensor);
  });

  /* Create 2 history line charts */
  histTempPres = createHistoryChart('chart-tp', [
    { label: 'Temperature (°C)', color: '#ff6b6b', unit: '°C'  },
    { label: 'Pressure (hPa)',   color: '#4ecdc4', unit: 'hPa' },
  ]);
  histRpmCurr = createHistoryChart('chart-rc', [
    { label: 'RPM',        color: '#96ceb4', unit: 'RPM' },
    { label: 'Current (A)', color: '#ffd93d', unit: 'A'   },
  ]);

  /* First fetch immediately */
  fetchAll();
  fetchNetInfo();
  fetchSystem();

  /* Periodic polling */
  pollTimer = setInterval(fetchAll,     POLL_MS);
  sysTimer  = setInterval(fetchSystem,  SYS_MS);
  /* Refresh NetInfo once a minute */
  setInterval(fetchNetInfo, 60_000);

  console.log('[STM32 Monitor] Dashboard initialised. Polling every', POLL_MS, 'ms');
}

/* Start when DOM is ready */
if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', init);
} else {
  init();
}
