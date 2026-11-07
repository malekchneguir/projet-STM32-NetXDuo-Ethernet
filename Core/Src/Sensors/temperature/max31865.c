/* ============================================================
 * @file    max31865.c
 * @brief   MAX31865 RTD-to-Digital Converter driver (PT100)
 *          SPI4 — PE2=SCK, PE5=MISO, PE6=MOSI, PE4=CS
 * ============================================================ */
#include "max31865.h"
#include <math.h>

/* Internal state */
static uint8_t s_fault = 0;

/* ── SPI helpers ──────────────────────────────────────────── */
static void CS_Low(void)  { HAL_GPIO_WritePin(MAX31865_CS_PORT, MAX31865_CS_PIN, GPIO_PIN_RESET); }
static void CS_High(void) { HAL_GPIO_WritePin(MAX31865_CS_PORT, MAX31865_CS_PIN, GPIO_PIN_SET);   }

static void MAX31865_WriteReg(uint8_t reg, uint8_t value)
{
  uint8_t tx[2] = { (uint8_t)(reg | 0x80U), value };
  CS_Low();
  HAL_SPI_Transmit(&MAX31865_SPI_HANDLE, tx, 2, 10);
  CS_High();
}

static uint8_t MAX31865_ReadReg(uint8_t reg)
{
  uint8_t tx = reg & 0x7FU;
  uint8_t rx = 0;
  CS_Low();
  HAL_SPI_Transmit(&MAX31865_SPI_HANDLE, &tx, 1, 10);
  HAL_SPI_Receive (&MAX31865_SPI_HANDLE, &rx, 1, 10);
  CS_High();
  return rx;
}

static void MAX31865_ReadRegs(uint8_t reg, uint8_t *buf, uint8_t len)
{
  uint8_t tx = reg & 0x7FU;
  CS_Low();
  HAL_SPI_Transmit(&MAX31865_SPI_HANDLE, &tx, 1, 10);
  HAL_SPI_Receive (&MAX31865_SPI_HANDLE, buf, len, 10);
  CS_High();
}

/* ── Public API ───────────────────────────────────────────── */

void MAX31865_Init(void)
{
  CS_High();
  HAL_Delay(10);
  /* Config: VBIAS ON, Auto-Conv, 2/4-wire, fault clear */
  MAX31865_WriteReg(MAX31865_REG_CONFIG,
                    MAX31865_CFG_VBIAS |
                    MAX31865_CFG_AUTOCONV |
                    MAX31865_CFG_FAULT_CLR);
  HAL_Delay(100); /* Allow VBIAS to settle */
}

float MAX31865_ReadTemperature(void)
{
  uint8_t buf[2];
  MAX31865_ReadRegs(MAX31865_REG_RTD_MSB, buf, 2);

  /* Bit0 of LSB is fault flag */
  if (buf[1] & 0x01U)
  {
    s_fault = MAX31865_ReadReg(MAX31865_REG_FAULT);
    return -999.0f;
  }
  s_fault = 0;

  /* 15-bit RTD value (shift out LSB fault bit) */
  uint16_t raw = ((uint16_t)buf[0] << 8U | buf[1]) >> 1U;

  /* Resistance of RTD: R = (raw / 32768) * R_REF */
  float R = ((float)raw * PT100_R_REFERENCE) / 32768.0f;

  /* Callendar-Van Dusen simplified (valid -200..+850°C) */
  /* T ≈ (R/R0 - 1) / alpha,  alpha = 0.003850 for PT100 */
  float temp = (R / PT100_R_NOMINAL - 1.0f) / 0.00385f;

  return temp;
}

uint8_t MAX31865_IsOk(void)
{
  return (s_fault == 0) ? 1U : 0U;
}

uint8_t MAX31865_ReadFault(void)
{
  return s_fault;
}
