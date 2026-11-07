/* ============================================================
 * @file    acs712.h
 * @brief   ACS712 Hall-Effect Current Sensor driver
 *          Interface: ADC3 Channel 7 — PF8
 *          ACS712-20A: 100 mV/A sensitivity, offset = Vcc/2 = 1.65V
 * ============================================================ */
#ifndef ACS712_H
#define ACS712_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* ADC configuration */
#define ACS712_ADC_HANDLE       hadc3
#define ACS712_ADC_CHANNEL      ADC_CHANNEL_7   /* PF8 */
#define ACS712_VCC              3.3f            /* Supply voltage (V) */
#define ACS712_SENSITIVITY_MV   100.0f          /* mV/A for ACS712-20A */
#define ACS712_ADC_RESOLUTION   65535.0f        /* 16-bit ADC           */
#define ACS712_SAMPLES          16U             /* Oversampling count   */

/* Public API */
void  ACS712_Init(void);
float ACS712_ReadCurrentA(void);    /* Returns current in Amperes */
float ACS712_ReadVoltage(void);     /* Returns raw voltage in V   */

/* Extern handle (defined in main.c) */
extern ADC_HandleTypeDef hadc3;

#ifdef __cplusplus
}
#endif

#endif /* ACS712_H */
