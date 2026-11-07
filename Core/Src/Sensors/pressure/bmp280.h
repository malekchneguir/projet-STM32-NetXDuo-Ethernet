/* ============================================================
 * @file    bmp280.h
 * @brief   BMP280 Pressure & Temperature Sensor driver
 *          Interface: I2C1 — PB6=SCL, PB7=SDA, Addr=0x76
 * ============================================================ */
#ifndef BMP280_H
#define BMP280_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* I2C address (SDO=GND → 0x76, SDO=VCC → 0x77) */
#define BMP280_I2C_HANDLE    hi2c1
#define BMP280_I2C_ADDR      (0x76 << 1)

/* Register map */
#define BMP280_REG_CALIB     0x88   /* Calibration data (24 bytes) */
#define BMP280_REG_ID        0xD0
#define BMP280_REG_RESET     0xE0
#define BMP280_REG_STATUS    0xF3
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG    0xF5
#define BMP280_REG_PRES_MSB  0xF7   /* 3-byte burst: P + T */

#define BMP280_CHIP_ID       0x60

/* Oversampling & operating mode */
#define BMP280_MODE_NORMAL   0x03
#define BMP280_OSRS_P_x8     (0x04 << 2)
#define BMP280_OSRS_T_x1     (0x01 << 5)

/* Public API */
void    BMP280_Init(void);
float   BMP280_ReadPressure(void);   /* Returns hPa */
float   BMP280_ReadTemperature(void);/* Returns °C  */
uint8_t BMP280_IsOk(void);

/* Extern handle (defined in main.c) */
extern I2C_HandleTypeDef hi2c1;

#ifdef __cplusplus
}
#endif

#endif /* BMP280_H */
