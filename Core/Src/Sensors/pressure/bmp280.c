/* ============================================================
 * @file    bmp280.c
 * @brief   BMP280 Pressure & Temperature driver
 *          I2C1 — PB6=SCL, PB7=SDA, default addr 0x76
 * ============================================================ */
#include "bmp280.h"

/* Compensation coefficients (loaded from OTP at init) */
typedef struct {
  uint16_t dig_T1; int16_t  dig_T2; int16_t  dig_T3;
  uint16_t dig_P1; int16_t  dig_P2; int16_t  dig_P3;
  int16_t  dig_P4; int16_t  dig_P5; int16_t  dig_P6;
  int16_t  dig_P7; int16_t  dig_P8; int16_t  dig_P9;
} BMP280_Calib_t;

static BMP280_Calib_t s_calib;
static uint8_t        s_ok = 0;
static int32_t        s_t_fine; /* shared between T and P compensation */

/* ── I2C helpers ──────────────────────────────────────────── */
static HAL_StatusTypeDef BMP280_WriteReg(uint8_t reg, uint8_t val)
{
  uint8_t buf[2] = { reg, val };
  return HAL_I2C_Master_Transmit(&BMP280_I2C_HANDLE, BMP280_I2C_ADDR,
                                  buf, 2, 10);
}

static HAL_StatusTypeDef BMP280_ReadRegs(uint8_t reg, uint8_t *buf, uint16_t len)
{
  HAL_I2C_Master_Transmit(&BMP280_I2C_HANDLE, BMP280_I2C_ADDR, &reg, 1, 10);
  return HAL_I2C_Master_Receive (&BMP280_I2C_HANDLE, BMP280_I2C_ADDR, buf, len, 10);
}

/* ── Public API ───────────────────────────────────────────── */

void BMP280_Init(void)
{
  uint8_t buf[24];

  /* Verify chip ID */
  BMP280_ReadRegs(BMP280_REG_ID, buf, 1);
  if (buf[0] != BMP280_CHIP_ID && buf[0] != 0x58) { s_ok = 0; return; }

  /* Load calibration OTP data */
  BMP280_ReadRegs(BMP280_REG_CALIB, buf, 24);
  s_calib.dig_T1 = (uint16_t)(buf[1]  << 8 | buf[0]);
  s_calib.dig_T2 = (int16_t) (buf[3]  << 8 | buf[2]);
  s_calib.dig_T3 = (int16_t) (buf[5]  << 8 | buf[4]);
  s_calib.dig_P1 = (uint16_t)(buf[7]  << 8 | buf[6]);
  s_calib.dig_P2 = (int16_t) (buf[9]  << 8 | buf[8]);
  s_calib.dig_P3 = (int16_t) (buf[11] << 8 | buf[10]);
  s_calib.dig_P4 = (int16_t) (buf[13] << 8 | buf[12]);
  s_calib.dig_P5 = (int16_t) (buf[15] << 8 | buf[14]);
  s_calib.dig_P6 = (int16_t) (buf[17] << 8 | buf[16]);
  s_calib.dig_P7 = (int16_t) (buf[19] << 8 | buf[18]);
  s_calib.dig_P8 = (int16_t) (buf[21] << 8 | buf[20]);
  s_calib.dig_P9 = (int16_t) (buf[23] << 8 | buf[22]);

  /* Normal mode, P x8 oversampling, T x1 oversampling */
  BMP280_WriteReg(BMP280_REG_CTRL_MEAS,
                  BMP280_OSRS_T_x1 | BMP280_OSRS_P_x8 | BMP280_MODE_NORMAL);
  BMP280_WriteReg(BMP280_REG_CONFIG, 0xA0); /* t_standby=1000ms, filter x16 */

  HAL_Delay(100);
  s_ok = 1;
}

/* Internal: read & compensate temperature (must call before pressure) */
static float BMP280_CompensateTemp(int32_t adc_T)
{
  int32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((int32_t)s_calib.dig_T1 << 1))) *
           ((int32_t)s_calib.dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)s_calib.dig_T1)) *
             ((adc_T >> 4) - ((int32_t)s_calib.dig_T1))) >> 12) *
           ((int32_t)s_calib.dig_T3)) >> 14;
  s_t_fine = var1 + var2;
  T = (s_t_fine * 5 + 128) >> 8;
  return (float)T / 100.0f;
}

float BMP280_ReadTemperature(void)
{
  if (!s_ok) return -999.0f;
  uint8_t buf[6];
  BMP280_ReadRegs(BMP280_REG_PRES_MSB, buf, 6);
  int32_t adc_T = ((int32_t)buf[3] << 12) | ((int32_t)buf[4] << 4) | (buf[5] >> 4);
  return BMP280_CompensateTemp(adc_T);
}

float BMP280_ReadPressure(void)
{
  if (!s_ok) return -1.0f;
  uint8_t buf[6];
  BMP280_ReadRegs(BMP280_REG_PRES_MSB, buf, 6);

  /* Must compensate T first to get t_fine */
  int32_t adc_T = ((int32_t)buf[3] << 12) | ((int32_t)buf[4] << 4) | (buf[5] >> 4);
  BMP280_CompensateTemp(adc_T);

  int32_t adc_P = ((int32_t)buf[0] << 12) | ((int32_t)buf[1] << 4) | (buf[2] >> 4);

  /* BMP280 pressure compensation (datasheet section 4.2.3) */
  int64_t var1, var2, p;
  var1 = ((int64_t)s_t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)s_calib.dig_P6;
  var2 = var2 + ((var1 * (int64_t)s_calib.dig_P5) << 17);
  var2 = var2 + (((int64_t)s_calib.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)s_calib.dig_P3) >> 8) +
         ((var1 * (int64_t)s_calib.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)s_calib.dig_P1) >> 33;
  if (var1 == 0) return 0.0f;
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)s_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)s_calib.dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)s_calib.dig_P7) << 4);

  /* Result in Pa, convert to hPa */
  return (float)(uint32_t)p / 25600.0f;
}

uint8_t BMP280_IsOk(void)
{
  return s_ok;
}
