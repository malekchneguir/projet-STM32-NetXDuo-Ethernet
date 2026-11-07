/* ============================================================
 * @file    sensor_manager.c
 * @brief   Industrial Sensor Manager Implementation
 *          ThreadX acquisition thread + simulation mode
 * ============================================================ */
#include "sensor_manager.h"
#include <string.h>
#include <math.h>

/* Include real drivers only when running on actual hardware */
#if !SENSOR_SIMULATION_MODE
  #include "temperature/max31865.h"
  #include "pressure/bmp280.h"
  #include "level/hcsr04.h"
  #include "rpm/encoder.h"
  #include "current/acs712.h"
  #include "gas/mq2.h"
  #include "light/bh1750.h"
#endif

/* ============================================================
 * Private variables — NOT exported
 * ============================================================ */
static TX_THREAD     s_sensor_thread;
static UCHAR         s_sensor_stack[SENSOR_THREAD_STACK_SIZE];
static SensorData_t  s_sensor_data;
static TX_MUTEX      s_sensor_mutex;
static uint32_t      s_uptime_ticks = 0;

/* ============================================================
 * SIMULATION ENGINE
 * Generates realistic time-varying sensor readings using
 * sinusoidal waves at different frequencies and phases.
 * Values are physically plausible and smoothly animated.
 * ============================================================ */
#if SENSOR_SIMULATION_MODE

static float s_sim_time = 0.0f;

/**
 * @brief  Clamp a float value between min and max.
 */
static float _clampf(float v, float lo, float hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

/**
 * @brief  Fill SensorData_t with simulated realistic values.
 *         Called every 500 ms → sim_time advances by 0.5 per call.
 */
static void Sensor_Simulate(SensorData_t *d)
{
  float t = s_sim_time;

  /* Temperature: 25°C ± 8°C, period ~80s */
  d->temperature  = 25.0f + 8.0f * sinf(t * 0.078f);

  /* Pressure: 1013 hPa ± 12 hPa, period ~125s */
  d->pressure     = 1013.0f + 12.0f * cosf(t * 0.050f);

  /* Level: 60 cm ± 40 cm, period ~90s */
  d->level        = 60.0f + 40.0f * sinf(t * 0.070f + 1.0f);
  d->level        = _clampf(d->level, 1.0f, 200.0f);

  /* RPM: 1200 ± 700, period ~52s */
  float rpm_f     = 1200.0f + 700.0f * sinf(t * 0.120f + 0.5f);
  d->rpm          = (uint32_t)_clampf(rpm_f, 0.0f, 3000.0f);

  /* Current: 3.0 A ± 2.5 A, period ~63s */
  d->current      = 3.0f + 2.5f * sinf(t * 0.100f + 2.1f);
  d->current      = _clampf(d->current, 0.0f, 20.0f);

  /* Gas: 40 % ± 35 %, period ~105s — using abs(sin) for realistic spikes */
  d->gas_pct      = 20.0f + 55.0f * fabsf(sinf(t * 0.060f + 1.5f));
  d->gas_pct      = _clampf(d->gas_pct, 0.0f, 100.0f);

  /* Light: 450 lux ± 400 lux, period ~157s (simulates day/night cycle) */
  d->light_lux    = 450.0f + 400.0f * (0.5f + 0.5f * sinf(t * 0.040f + 0.8f));
  d->light_lux    = _clampf(d->light_lux, 0.0f, 1200.0f);

  /* All sensors OK in simulation */
  d->temp_ok = d->pressure_ok = d->level_ok = 1;
  d->rpm_ok  = d->current_ok  = d->gas_ok   = d->light_ok = 1;

  /* Derived alerts */
  d->gas_alert     = (d->gas_pct    > GAS_ALERT_THRESHOLD_PCT)  ? 1U : 0U;
  d->temp_alert    = (d->temperature > TEMP_ALERT_HIGH_C)        ? 1U : 0U;
  d->current_alert = (d->current    > CURRENT_ALERT_HIGH_A)      ? 1U : 0U;

  /* Advance simulation clock by 0.5s */
  s_sim_time += 0.5f;
}

#endif /* SENSOR_SIMULATION_MODE */

/* ============================================================
 * Acquisition Thread Entry
 * ============================================================ */
static void SensorAcq_Thread_Entry(ULONG arg)
{
  (void)arg;

  /* ── Hardware initialisation (real mode only) ─────────── */
#if !SENSOR_SIMULATION_MODE
  MAX31865_Init();
  BMP280_Init();
  HCSR04_Init();
  Encoder_Init();
  ACS712_Init();
  MQ2_Init();
  BH1750_Init();
#endif

  /* ── Main acquisition loop ────────────────────────────── */
  while (1)
  {
    SensorData_t new_data;
    memset(&new_data, 0, sizeof(new_data));

    /* Update uptime (each sleep ≈ 500 ms) */
    s_uptime_ticks++;
    new_data.uptime_s = s_uptime_ticks / 2U; /* 2 cycles = 1 second */

#if SENSOR_SIMULATION_MODE
    /* ── Simulated readings ──────────────────────────────── */
    Sensor_Simulate(&new_data);
    new_data.acq_count = s_uptime_ticks;

#else
    /* ── Real hardware readings ──────────────────────────── */
    new_data.temperature  = MAX31865_ReadTemperature();
    new_data.temp_ok      = MAX31865_IsOk();
    if (!new_data.temp_ok) new_data.temperature = 0.0f;

    new_data.pressure     = BMP280_ReadPressure();
    new_data.pressure_ok  = BMP280_IsOk();
    if (!new_data.pressure_ok) new_data.pressure = 0.0f;

    new_data.level        = HCSR04_ReadDistanceCm();
    new_data.level_ok     = (new_data.level >= 0.0f) ? 1U : 0U;
    if (new_data.level < 0.0f) new_data.level = 0.0f;

    new_data.rpm          = Encoder_ReadRPM();
    new_data.rpm_ok       = 1U;

    new_data.current      = ACS712_ReadCurrentA();
    new_data.current_ok   = 1U;
    if (new_data.current < 0.2f) new_data.current = 0.0f; /* Ignore hardware noise near 0 */

    new_data.gas_pct      = MQ2_ReadPercent();
    new_data.gas_ok       = 1U;
    if (new_data.gas_pct < 1.0f) new_data.gas_pct = 0.0f; /* Deadband for floating/clean air */

    new_data.light_lux    = BH1750_ReadLux();
    new_data.light_ok     = (new_data.light_lux >= 0.0f) ? 1U : 0U;
    if (!new_data.light_ok) new_data.light_lux = 0.0f;

    /* Derived alerts */
    new_data.gas_alert     = (new_data.gas_pct    > GAS_ALERT_THRESHOLD_PCT) ? 1U : 0U;
    new_data.temp_alert    = (new_data.temperature > TEMP_ALERT_HIGH_C)       ? 1U : 0U;
    new_data.current_alert = (new_data.current    > CURRENT_ALERT_HIGH_A)     ? 1U : 0U;
    new_data.acq_count     = s_uptime_ticks;
#endif

    /* ── Thread-safe write to shared data ────────────────── */
    if (tx_mutex_get(&s_sensor_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      memcpy(&s_sensor_data, &new_data, sizeof(SensorData_t));
      tx_mutex_put(&s_sensor_mutex);
    }

    /* ── Wait until next acquisition ─────────────────────── */
    tx_thread_sleep(SENSOR_ACQ_PERIOD_TICKS);
  }
}

/* ============================================================
 * Public Functions
 * ============================================================ */

void Sensor_Init(TX_BYTE_POOL *byte_pool)
{
  (void)byte_pool; /* Static stack — byte pool not needed here */

  /* Initialize mutex */
  tx_mutex_create(&s_sensor_mutex, "Sensor Data Mutex", TX_NO_INHERIT);

  /* Zero initial data */
  memset(&s_sensor_data, 0, sizeof(s_sensor_data));

  /* Create acquisition thread (auto-start) */
  tx_thread_create(&s_sensor_thread,
                   "Sensor Acquisition Thread",
                   SensorAcq_Thread_Entry,
                   0U,
                   s_sensor_stack,
                   SENSOR_THREAD_STACK_SIZE,
                   SENSOR_THREAD_PRIORITY,
                   SENSOR_THREAD_PRIORITY,
                   TX_NO_TIME_SLICE,
                   TX_AUTO_START);
}

void Sensor_GetData(SensorData_t *out)
{
  if (out == NULL) return;
  if (tx_mutex_get(&s_sensor_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
  {
    memcpy(out, &s_sensor_data, sizeof(SensorData_t));
    tx_mutex_put(&s_sensor_mutex);
  }
}

TX_THREAD *Sensor_GetThread(void)
{
  return &s_sensor_thread;
}

TX_MUTEX *Sensor_GetMutex(void)
{
  return &s_sensor_mutex;
}
