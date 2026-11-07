/* ============================================================
 * @file    bh1750.c
 * @brief   BH1750FVI Ambient Light Sensor driver
 *          I2C1 — PB6=SCL, PB7=SDA, Addr=0x23
 * ============================================================ */
#include "bh1750.h"

static uint8_t s_ok = 0;

static HAL_StatusTypeDef BH1750_SendCmd(uint8_t cmd)
{
  return HAL_I2C_Master_Transmit(&BH1750_I2C_HANDLE, BH1750_I2C_ADDR,
                                  &cmd, 1, 10);
}

void BH1750_Init(void)
{
  HAL_StatusTypeDef status;
  status  = BH1750_SendCmd(BH1750_CMD_POWER_ON);
  status |= BH1750_SendCmd(BH1750_CMD_RESET);
  HAL_Delay(10);
  /* Start continuous high-resolution mode (1 lux, ~120ms measurement time) */
  status |= BH1750_SendCmd(BH1750_CMD_CONT_H_RES);
  HAL_Delay(BH1750_MEAS_TIMEOUT_MS);
  s_ok = (status == HAL_OK) ? 1U : 0U;
}

float BH1750_ReadLux(void)
{
  if (!s_ok) return -1.0f;

  uint8_t buf[2] = {0};
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(
    &BH1750_I2C_HANDLE, BH1750_I2C_ADDR, buf, 2, 10);

  if (status != HAL_OK) { s_ok = 0; return -1.0f; }

  /* Raw value = (MSB << 8 | LSB) */
  /* Lux = raw / 1.2  (from BH1750 datasheet) */
  uint16_t raw = ((uint16_t)buf[0] << 8U) | buf[1];
  return (float)raw / 1.2f;
}
