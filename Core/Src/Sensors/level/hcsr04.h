/* ============================================================
 * @file    hcsr04.h
 * @brief   HC-SR04 Ultrasonic Distance Sensor driver
 *          TRIG: PF6 (GPIO output 10µs pulse)
 *          ECHO: PF7 (TIM2 CH1 Input Capture)
 * ============================================================ */
#ifndef HCSR04_H
#define HCSR04_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* GPIO & Timer configuration */
#define HCSR04_TRIG_PORT       GPIOF
#define HCSR04_TRIG_PIN        GPIO_PIN_6
#define HCSR04_ECHO_TIM        htim2
#define HCSR04_ECHO_CHANNEL    TIM_CHANNEL_1
#define HCSR04_TIMEOUT_US      30000U   /* 30 ms max echo timeout  */
#define HCSR04_MAX_DISTANCE_CM 400.0f   /* Physical sensor limit    */
#define HCSR04_MIN_DISTANCE_CM 2.0f     /* Physical sensor limit    */

/* Public API */
void  HCSR04_Init(void);
float HCSR04_ReadDistanceCm(void);  /* Returns -1.0 on timeout */

/* Extern handle (defined in main.c) */
extern TIM_HandleTypeDef htim2;

#ifdef __cplusplus
}
#endif

#endif /* HCSR04_H */
