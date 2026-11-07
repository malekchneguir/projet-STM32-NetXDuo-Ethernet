/* ============================================================
 * @file    acs712.c
 * @brief   ACS712 20A Hall-Effect Current Sensor driver
 *          ADC3 Channel 7 — PF8
 *          Sensitivity: 100 mV/A, offset at Vcc/2 = 1.65V
 * ============================================================ */
#include "acs712.h"

void ACS712_Init(void)
{
  /* ADC3 channel 7 initialised via HAL_ADC_MspInit in main */
  HAL_ADCEx_Calibration_Start(&ACS712_ADC_HANDLE, ADC_CALIB_OFFSET,
                               ADC_SINGLE_ENDED);
}

float ACS712_ReadVoltage(void)
{
  uint32_t adc_sum = 0;

  for (uint32_t i = 0; i < ACS712_SAMPLES; i++)
  {
    HAL_ADC_Start(&ACS712_ADC_HANDLE);
    HAL_ADC_PollForConversion(&ACS712_ADC_HANDLE, 10);
    adc_sum += HAL_ADC_GetValue(&ACS712_ADC_HANDLE);
    HAL_ADC_Stop(&ACS712_ADC_HANDLE);
  }

  float adc_avg = (float)adc_sum / (float)ACS712_SAMPLES;
  /* Convert to voltage: V = (raw / max_raw) * Vcc */
  return (adc_avg / ACS712_ADC_RESOLUTION) * ACS712_VCC;
}

float ACS712_ReadCurrentA(void)
{
  float voltage = ACS712_ReadVoltage();
  /* Current = (V_sensor - V_offset) / sensitivity */
  /* V_offset = Vcc / 2 = 1.65V at zero current     */
  float offset_v   = ACS712_VCC / 2.0f;
  float current_a  = (voltage - offset_v) / (ACS712_SENSITIVITY_MV / 1000.0f);
  return current_a;
}
