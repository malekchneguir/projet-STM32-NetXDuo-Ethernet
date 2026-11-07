/* ============================================================
 * @file    hcsr04.c
 * @brief   HC-SR04 Ultrasonic Distance Sensor driver
 *          TRIG: PF6 (10µs GPIO pulse), ECHO: PF7 (input capture)
 * ============================================================ */
#include "hcsr04.h"

/* ── Public API ───────────────────────────────────────────── */

void HCSR04_Init(void)
{
  /* GPIO for TRIG is initialised in HAL_MSP (output push-pull PF6) */
  /* TIM2 CH1 Input Capture is started here */
  HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
  HAL_TIM_IC_Start(&HCSR04_ECHO_TIM, HCSR04_ECHO_CHANNEL);
}

/**
 * @brief  Blocking measurement (max 30 ms wait).
 *         Uses HAL_GetTick() for timeout — safe inside ThreadX tasks.
 * @return Distance in cm, or -1.0f on timeout/no echo.
 */
float HCSR04_ReadDistanceCm(void)
{
  uint32_t t_start, t_echo_start, t_echo_end;
  uint32_t timeout = HCSR04_TIMEOUT_US / 1000U + 5U; /* ms */

  /* ── Send 10 µs TRIG pulse ──── */
  HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_SET);
  /* Busy-wait 10 µs — minimal HAL_Delay is 1 ms; use DWT if available */
  /* For STM32H7 running at 240 MHz: 10µs ≈ 2400 cycles */
  volatile uint32_t cycles = 2400U;
  while (cycles--) { __NOP(); }
  HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);

  /* ── Wait for ECHO rising edge ──── */
  t_start = HAL_GetTick();
  /* Read ECHO pin directly for timing (PF7 configured as input) */
  while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7) == GPIO_PIN_RESET)
  {
    if ((HAL_GetTick() - t_start) > timeout) return -1.0f;
  }
  t_echo_start = HAL_GetTick();

  /* ── Wait for ECHO falling edge ──── */
  while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7) == GPIO_PIN_SET)
  {
    if ((HAL_GetTick() - t_echo_start) > timeout) return -1.0f;
  }
  t_echo_end = HAL_GetTick();

  /* ── Convert duration to cm ──── */
  /* Distance = (time_us * speed_of_sound) / 2  */
  /* HAL_GetTick() resolution = 1ms → multiply by 1000 for µs */
  float duration_us = (float)(t_echo_end - t_echo_start) * 1000.0f;
  float distance_cm = (duration_us * 0.0343f) / 2.0f;

  /* Clamp to sensor spec */
  if (distance_cm < HCSR04_MIN_DISTANCE_CM) return HCSR04_MIN_DISTANCE_CM;
  if (distance_cm > HCSR04_MAX_DISTANCE_CM) return HCSR04_MAX_DISTANCE_CM;

  return distance_cm;
}
