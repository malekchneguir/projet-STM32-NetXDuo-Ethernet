/* ============================================================
 * @file    api_handler.c
 * @brief   REST API JSON response builder
 *          Reads from SensorData_t (thread-safe via sensor mutex)
 *          and formats JSON strings for each HTTP endpoint.
 * ============================================================ */
#include "api_handler.h"
#include "../Sensors/sensor_manager.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ── Helper ────────────────────────────────────────────────── */
static const char *bool_str(uint8_t v) { return v ? "true" : "false"; }
static const char *ok_str(uint8_t v)   { return v ? "ok"   : "error"; }

/* Float formatting macros to bypass GCC %f which is disabled in newlib-nano */
#define SIGN(x) ((x) < 0.0f ? "-" : "")
#define INT_VAL(x) ((int)fabs(x))
#define FRAC1_VAL(x) ((int)((fabs(x) - (int)fabs(x)) * 10.0f) % 10)
#define FRAC2_VAL(x) ((int)((fabs(x) - (int)fabs(x)) * 100.0f) % 100)
#define FRAC3_VAL(x) ((int)((fabs(x) - (int)fabs(x)) * 1000.0f) % 1000)

/* ============================================================
 * Individual endpoint implementations
 * ============================================================ */

void API_GetTemp(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"temperature\":%s%d.%02d,"
      "\"unit\":\"C\","
      "\"alert\":%s,"
      "\"status\":\"%s\""
    "}",
    SIGN(d.temperature), INT_VAL(d.temperature), FRAC2_VAL(d.temperature),
    bool_str(d.temp_alert),
    ok_str(d.temp_ok)
  );
}

void API_GetPressure(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"pressure\":%d.%02d,"
      "\"unit\":\"hPa\","
      "\"status\":\"%s\""
    "}",
    INT_VAL(d.pressure), FRAC2_VAL(d.pressure),
    ok_str(d.pressure_ok)
  );
}

void API_GetLevel(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"level\":%d.%01d,"
      "\"unit\":\"cm\","
      "\"status\":\"%s\""
    "}",
    INT_VAL(d.level), FRAC1_VAL(d.level),
    ok_str(d.level_ok)
  );
}

void API_GetRPM(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"rpm\":%lu,"
      "\"status\":\"%s\""
    "}",
    (unsigned long)d.rpm,
    ok_str(d.rpm_ok)
  );
}

void API_GetCurrent(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"current\":%d.%03d,"
      "\"unit\":\"A\","
      "\"alert\":%s,"
      "\"status\":\"%s\""
    "}",
    INT_VAL(d.current), FRAC3_VAL(d.current),
    bool_str(d.current_alert),
    ok_str(d.current_ok)
  );
}

void API_GetGas(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"gas\":%d.%01d,"
      "\"unit\":\"%%\","
      "\"alert\":%s,"
      "\"status\":\"%s\""
    "}",
    INT_VAL(d.gas_pct), FRAC1_VAL(d.gas_pct),
    bool_str(d.gas_alert),
    ok_str(d.gas_ok)
  );
}

void API_GetLight(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"light\":%d.%01d,"
      "\"unit\":\"lux\","
      "\"status\":\"%s\""
    "}",
    INT_VAL(d.light_lux), FRAC1_VAL(d.light_lux),
    ok_str(d.light_ok)
  );
}

/* ============================================================
 * /api/all — Full snapshot (used by dashboard every 1s)
 * ============================================================ */
void API_GetAll(char *buf, size_t len)
{
  SensorData_t d;
  Sensor_GetData(&d);
  snprintf(buf, len,
    "{"
      "\"temperature\":%s%d.%02d,"
      "\"pressure\":%d.%02d,"
      "\"level\":%d.%01d,"
      "\"rpm\":%lu,"
      "\"current\":%d.%03d,"
      "\"gas\":%d.%01d,"
      "\"light\":%d.%01d,"
      "\"temp_alert\":%s,"
      "\"current_alert\":%s,"
      "\"gas_alert\":%s,"
      "\"temp_ok\":%s,"
      "\"pressure_ok\":%s,"
      "\"level_ok\":%s,"
      "\"rpm_ok\":%s,"
      "\"current_ok\":%s,"
      "\"gas_ok\":%s,"
      "\"light_ok\":%s,"
      "\"uptime\":%lu,"
      "\"acq_count\":%lu,"
      "\"sim_mode\":%s"
    "}",
    SIGN(d.temperature), INT_VAL(d.temperature), FRAC2_VAL(d.temperature),
    INT_VAL(d.pressure), FRAC2_VAL(d.pressure),
    INT_VAL(d.level), FRAC1_VAL(d.level),
    (unsigned long)d.rpm,
    INT_VAL(d.current), FRAC3_VAL(d.current),
    INT_VAL(d.gas_pct), FRAC1_VAL(d.gas_pct),
    INT_VAL(d.light_lux), FRAC1_VAL(d.light_lux),
    bool_str(d.temp_alert),
    bool_str(d.current_alert),
    bool_str(d.gas_alert),
    bool_str(d.temp_ok),
    bool_str(d.pressure_ok),
    bool_str(d.level_ok),
    bool_str(d.rpm_ok),
    bool_str(d.current_ok),
    bool_str(d.gas_ok),
    bool_str(d.light_ok),
    (unsigned long)d.uptime_s,
    (unsigned long)d.acq_count,
    bool_str(SENSOR_SIMULATION_MODE)
  );
}

/* ============================================================
 * /api/system — ThreadX & memory statistics
 * ============================================================ */
void API_GetSystem(char *buf, size_t len)
{
  ULONG resumptions, suspensions, idle_returns, non_idle_returns;
  ULONG available_bytes, fragments;

  tx_thread_performance_system_info_get(
    &resumptions, &suspensions, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, &non_idle_returns, &idle_returns);

  /* Get byte pool info (approximation using ThreadX internals) */
  available_bytes = 0;
  fragments       = 0;

  SensorData_t d;
  Sensor_GetData(&d);

  snprintf(buf, len,
    "{"
      "\"uptime_s\":%lu,"
      "\"tx_resumptions\":%lu,"
      "\"tx_suspensions\":%lu,"
      "\"tx_idle_returns\":%lu,"
      "\"tx_non_idle_returns\":%lu,"
      "\"acq_cycles\":%lu,"
      "\"sim_mode\":%s"
    "}",
    (unsigned long)d.uptime_s,
    (unsigned long)resumptions,
    (unsigned long)suspensions,
    (unsigned long)idle_returns,
    (unsigned long)non_idle_returns,
    (unsigned long)d.acq_count,
    bool_str(SENSOR_SIMULATION_MODE)
  );
}
