/* ============================================================
 * @file    mq2.c
 * @brief   MQ2 Gas Sensor driver (LPG, Smoke, CO, Propane)
 *          ADC3 Channel 2 — PF9
 *          Output: 0-100% relative concentration
 * ============================================================ */
#include "mq2.h"

void MQ2_Init(void)
{
  /* ADC3 shared with ACS712 — already calibrated if ACS712_Init runs first */
  /* Add 20s preheat delay in real application before first reading */
}

float MQ2_ReadRawVoltage(void)
{
  uint32_t adc_sum = 0;
  ADC_ChannelConfTypeDef ch_cfg = {0};

  /* Switch ADC3 to MQ2 channel */
  ch_cfg.Channel      = MQ2_ADC_CHANNEL;
  ch_cfg.Rank         = ADC_REGULAR_RANK_1;
  ch_cfg.SamplingTime = ADC_SAMPLETIME_387CYCLES_5;
  HAL_ADC_ConfigChannel(&MQ2_ADC_HANDLE, &ch_cfg);

  for (uint32_t i = 0; i < MQ2_SAMPLES; i++)
  {
    HAL_ADC_Start(&MQ2_ADC_HANDLE);
    HAL_ADC_PollForConversion(&MQ2_ADC_HANDLE, 10);
    adc_sum += HAL_ADC_GetValue(&MQ2_ADC_HANDLE);
    HAL_ADC_Stop(&MQ2_ADC_HANDLE);
  }

  float adc_avg = (float)adc_sum / (float)MQ2_SAMPLES;
  /* V = (raw / max_raw) * 3.3V */
  return (adc_avg / MQ2_ADC_RESOLUTION) * 3.3f;
}

float MQ2_ReadPercent(void)
{
  float voltage = MQ2_ReadRawVoltage();
  /* Sensor output voltage increases with gas concentration.
   * Clean air ≈ 0.1V, saturated ≈ 3.2V
   * Simple linear mapping to 0-100% */
  float pct = ((voltage - 0.1f) / (3.2f - 0.1f)) * 100.0f;
  if (pct < 0.0f)   pct = 0.0f;
  if (pct > 100.0f) pct = 100.0f;
  return pct;
}
