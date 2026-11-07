/* ============================================================
 * @file    encoder.h
 * @brief   Incremental Encoder RPM driver
 *          Interface: TIM3 Encoder Mode
 *          TI1: PB4 (AF2), TI2: PB5 (AF2)
 * ============================================================ */
#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* Timer configuration */
#define ENCODER_TIM_HANDLE    htim3
#define ENCODER_PPR           1000U     /* Pulses per revolution (adjust to your encoder) */
#define ENCODER_SAMPLE_MS     500U     /* Sampling interval in ms (matches sensor acq)    */

/* Public API */
void     Encoder_Init(void);
uint32_t Encoder_ReadRPM(void);   /* Returns RPM (revolutions per minute) */
int32_t  Encoder_ReadCount(void); /* Raw encoder count since last read     */

/* Extern handle (defined in main.c) */
extern TIM_HandleTypeDef htim3;

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H */
