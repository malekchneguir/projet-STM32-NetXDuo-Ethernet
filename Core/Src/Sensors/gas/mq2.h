/* ============================================================
 * @file    mq2.h
 * @brief   MQ2 Gas Sensor driver (LPG, Smoke, CO, Propane)
 *          Interface: ADC3 Channel 2 — PF9
 *          Output: 0-100% concentration (relative to clean air baseline)
 * ============================================================ */
#ifndef MQ2_H
#define MQ2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* ADC configuration */
#define MQ2_ADC_HANDLE          hadc3
#define MQ2_ADC_CHANNEL         ADC_CHANNEL_2   /* PF9 */
#define MQ2_ADC_RESOLUTION      65535.0f        /* 16-bit */
#define MQ2_SAMPLES             16U             /* Oversampling */

/* Calibration — adjust to your environment/preheat */
#define MQ2_CLEAN_AIR_RATIO     9.6f   /* Rs/Ro in clean air */
#define MQ2_LOAD_RESISTANCE     10.0f  /* kΩ */

/* Alert threshold (%) — also checked in sensor_manager.h */
#define MQ2_ALERT_THRESHOLD     75.0f

/* Public API */
void  MQ2_Init(void);
float MQ2_ReadPercent(void);        /* Returns 0-100% */
float MQ2_ReadRawVoltage(void);     /* Returns raw ADC voltage in V */

/* Extern handle (defined in main.c) */
extern ADC_HandleTypeDef hadc3;

#ifdef __cplusplus
}
#endif

#endif /* MQ2_H */
