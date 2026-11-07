/* ============================================================
 * @file    sensor_manager.h
 * @brief   Industrial Sensor Manager — Data structures & public API
 *          Supports 7 industrial sensors with ThreadX synchronization.
 *          Compatible with Azure RTOS ThreadX + NetXDuo stack.
 * ============================================================ */
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tx_api.h"
#include <stdint.h>

/* ============================================================
 * SIMULATION MODE
 * 1 = Use simulated sensor data (no real hardware required)
 * 0 = Use real hardware drivers via HAL
 * ============================================================ */
#define SENSOR_SIMULATION_MODE 0

/* ============================================================
 * ThreadX Task Configuration
 * ============================================================ */
#define SENSOR_THREAD_STACK_SIZE   2048
#define SENSOR_THREAD_PRIORITY     6       /* Between NxApp(10) and AppServer(5) */
#define SENSOR_ACQ_PERIOD_TICKS    50      /* 500ms @ ThreadX 100 ticks/s        */

/* ============================================================
 * Thresholds & limits
 * ============================================================ */
#define GAS_ALERT_THRESHOLD_PCT    75.0f   /* Gas alert above 75%     */
#define TEMP_ALERT_HIGH_C          80.0f   /* Temp alert above 80°C   */
#define CURRENT_ALERT_HIGH_A       8.0f    /* Current alert above 8 A */

/* ============================================================
 * Sensor Data Structure — Thread-safe via SensorDataMutex
 * ============================================================ */
typedef struct
{
  /* ── Raw measurements ─────────────────────────────────── */
  float    temperature;   /* °C   ← MAX31865 / PT100 via SPI4     */
  float    pressure;      /* hPa  ← BMP280 via I2C1               */
  float    level;         /* cm   ← HC-SR04 via GPIO+TIM2         */
  uint32_t rpm;           /* RPM  ← Incremental Encoder via TIM3  */
  float    current;       /* A    ← ACS712 via ADC3               */
  float    gas_pct;       /* %    ← MQ2 via ADC3                  */
  float    light_lux;     /* lux  ← BH1750 via I2C1               */

  /* ── Per-sensor health: 1=OK, 0=Fault ────────────────── */
  uint8_t  temp_ok;
  uint8_t  pressure_ok;
  uint8_t  level_ok;
  uint8_t  rpm_ok;
  uint8_t  current_ok;
  uint8_t  gas_ok;
  uint8_t  light_ok;

  /* ── Alerts ────────────────────────────────────────────── */
  uint8_t  gas_alert;       /* 1 if gas_pct    > GAS_ALERT_THRESHOLD_PCT */
  uint8_t  temp_alert;      /* 1 if temperature > TEMP_ALERT_HIGH_C      */
  uint8_t  current_alert;   /* 1 if current     > CURRENT_ALERT_HIGH_A   */

  /* ── System ────────────────────────────────────────────── */
  uint32_t uptime_s;        /* Seconds elapsed since boot               */
  uint32_t acq_count;       /* Total acquisition cycles performed        */

} SensorData_t;

/* ============================================================
 * Public API
 * ============================================================ */

/**
 * @brief  Initialize sensor manager: creates mutex and acquisition thread.
 *         Call once from MX_NetXDuo_Init() after byte_pool is ready.
 * @param  byte_pool  Pointer to ThreadX byte pool for stack allocation
 */
void Sensor_Init(TX_BYTE_POOL *byte_pool);

/**
 * @brief  Thread-safe copy of the latest sensor data snapshot.
 * @param  out  Pointer to caller-owned SensorData_t to fill.
 */
void Sensor_GetData(SensorData_t *out);

/**
 * @brief  Returns pointer to the internal acquisition thread handle.
 */
TX_THREAD *Sensor_GetThread(void);

/**
 * @brief  Returns pointer to the internal sensor mutex (for advanced use).
 */
TX_MUTEX  *Sensor_GetMutex(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MANAGER_H */
