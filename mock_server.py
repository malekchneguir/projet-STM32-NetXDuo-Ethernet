"""
mock_server.py — Serveur de test local pour STM32 Industrial Monitor Dashboard
Simule exactement les mêmes réponses JSON que le STM32H753 en mode SIMULATION.

Usage:
    python mock_server.py
Puis ouvrir: http://localhost:8080
"""

import http.server
import json
import math
import time
import os

# ── Configuration ──────────────────────────────────────────
PORT      = 8080
WEB_DIR   = os.path.join(os.path.dirname(__file__), "WebDashboard")
START_TIME = time.time()

# ── Simulation engine (identique au firmware STM32) ────────
def simulate_sensors():
    t = (time.time() - START_TIME)   # temps en secondes

    temperature = 25.0 + 8.0  * math.sin(t * 0.078)
    pressure    = 1013.0 + 12.0 * math.cos(t * 0.050)
    level       = 60.0 + 40.0  * math.sin(t * 0.070 + 1.0)
    level       = max(1.0, min(200.0, level))
    rpm         = int(max(0, 1200.0 + 700.0 * math.sin(t * 0.120 + 0.5)))
    current     = max(0.0, 3.0 + 2.5  * math.sin(t * 0.100 + 2.1))
    gas         = max(0.0, min(100.0, 20.0 + 55.0 * abs(math.sin(t * 0.060 + 1.5))))
    light       = max(0.0, 450.0 + 400.0 * (0.5 + 0.5 * math.sin(t * 0.040 + 0.8)))
    uptime      = int(t)

    gas_alert     = gas     > 75.0
    temp_alert    = temperature > 80.0
    current_alert = current > 8.0

    return {
        "temperature":   round(temperature, 2),
        "pressure":      round(pressure, 2),
        "level":         round(level, 1),
        "rpm":           rpm,
        "current":       round(current, 3),
        "gas":           round(gas, 1),
        "light_lux":     round(light, 1),
        "temp_alert":    temp_alert,
        "current_alert": current_alert,
        "gas_alert":     gas_alert,
        "temp_ok":       True,
        "pressure_ok":   True,
        "level_ok":      True,
        "rpm_ok":        True,
        "current_ok":    True,
        "gas_ok":        True,
        "light_ok":      True,
        "uptime":        uptime,
        "acq_count":     uptime * 2,
        "sim_mode":      True,
    }

def get_system_stats():
    uptime = int(time.time() - START_TIME)
    return {
        "uptime_s":            uptime,
        "tx_resumptions":      12340 + uptime * 4,
        "tx_suspensions":      8920  + uptime * 3,
        "tx_idle_returns":     5600  + uptime * 2,
        "tx_non_idle_returns": 2700  + uptime,
        "acq_cycles":          uptime * 2,
        "sim_mode":            True,
    }

# ── HTTP Handler ────────────────────────────────────────────
class DashboardHandler(http.server.SimpleHTTPRequestHandler):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB_DIR, **kwargs)

    def do_GET(self):
        # ── API routes ──────────────────────────────────────
        if self.path == "/api/all":
            self._send_json(simulate_sensors())

        elif self.path == "/api/system":
            self._send_json(get_system_stats())

        elif self.path == "/api/temp":
            d = simulate_sensors()
            self._send_json({"temperature": d["temperature"], "unit": "C",
                             "alert": d["temp_alert"], "status": "ok"})

        elif self.path == "/api/pressure":
            d = simulate_sensors()
            self._send_json({"pressure": d["pressure"], "unit": "hPa", "status": "ok"})

        elif self.path == "/api/level":
            d = simulate_sensors()
            self._send_json({"level": d["level"], "unit": "cm", "status": "ok"})

        elif self.path == "/api/rpm":
            d = simulate_sensors()
            self._send_json({"rpm": d["rpm"], "status": "ok"})

        elif self.path == "/api/current":
            d = simulate_sensors()
            self._send_json({"current": d["current"], "unit": "A",
                             "alert": d["current_alert"], "status": "ok"})

        elif self.path == "/api/gas":
            d = simulate_sensors()
            self._send_json({"gas": d["gas"], "unit": "%",
                             "alert": d["gas_alert"], "status": "ok"})

        elif self.path == "/api/light":
            d = simulate_sensors()
            self._send_json({"light": d["light_lux"], "unit": "lux", "status": "ok"})

        elif self.path == "/GetNetInfo":
            # Simule la réponse de l'IP du STM32
            self._send_text("192.168.1.100,80")

        elif self.path == "/GetTXData":
            u = int(time.time() - START_TIME)
            self._send_text(f"{12340+u*4},{8920+u*3},{5600+u*2},{2700+u}")

        else:
            # Sert les fichiers statiques (index.html, style.css, app.js)
            super().do_GET()

    def _send_json(self, data):
        body = json.dumps(data).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type",  "application/json")
        self.send_header("Content-Length", len(body))
        self.send_header("Access-Control-Allow-Origin", "*")  # CORS
        self.end_headers()
        self.wfile.write(body)

    def _send_text(self, text):
        body = text.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type",  "text/plain")
        self.send_header("Content-Length", len(body))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, fmt, *args):
        # Filtre les logs /api/all (trop fréquents)
        if "/api/all" not in args[0] and "/api/system" not in args[0]:
            super().log_message(fmt, *args)

# ── Main ────────────────────────────────────────────────────
if __name__ == "__main__":
    if not os.path.isdir(WEB_DIR):
        print(f"[ERREUR] Dossier WebDashboard introuvable: {WEB_DIR}")
        print(f"         Assurez-vous que mock_server.py est dans NetXDuoEthernet/")
        exit(1)

    print("=" * 60)
    print("  STM32 Industrial Monitor — Mock Server")
    print("=" * 60)
    print(f"  Dossier web : {WEB_DIR}")
    print(f"  Dashboard   : http://localhost:{PORT}")
    print(f"  API test    : http://localhost:{PORT}/api/all")
    print(f"  Mode        : SIMULATION (données synthétiques)")
    print("=" * 60)
    print("  Ctrl+C pour arrêter\n")

    with http.server.ThreadingHTTPServer(("", PORT), DashboardHandler) as server:
        try:
            server.serve_forever()
        except KeyboardInterrupt:
            print("\n[INFO] Serveur arrêté.")
