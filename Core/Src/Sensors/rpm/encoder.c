/* ============================================================
 * @file    encoder.c
 * @brief   Incremental Encoder driver using TIM3 Encoder Mode
 *          TI1: PB4 (AF2), TI2: PB5 (AF2)
 * ============================================================ */
#include "encoder.h"

static uint32_t s_last_count = 0;

void Encoder_Init(void)
{
  /* TIM3 must be configured in Encoder mode (TI1+TI2) via HAL_MSP */
  HAL_TIM_Encoder_Start(&ENCODER_TIM_HANDLE, TIM_CHANNEL_ALL);
  s_last_count = __HAL_TIM_GET_COUNTER(&ENCODER_TIM_HANDLE);
}

int32_t Encoder_ReadCount(void)
{
  uint32_t current = __HAL_TIM_GET_COUNTER(&ENCODER_TIM_HANDLE);
  int32_t  delta   = (int32_t)(current - s_last_count);
  s_last_count     = current;
  return delta;
}

uint32_t Encoder_ReadRPM(void)
{
  int32_t delta = Encoder_ReadCount(); /* pulses in last ENCODER_SAMPLE_MS */

  /* RPM = (|delta| pulses / PPR) * (60000 ms / sample_ms) */
  if (delta < 0) delta = -delta; /* take absolute value */

  uint32_t rpm = ((uint32_t)delta * 60000U) /
                 (ENCODER_PPR * ENCODER_SAMPLE_MS);
  return rpm;
}
