/* ============================================================
 * @file    bh1750.h
 * @brief   BH1750FVI Ambient Light Sensor driver
 *          Interface: I2C1 — PB6=SCL, PB7=SDA, Addr=0x23 (ADDR=LOW)
 *          Range: 1 - 65535 lux, Resolution: 1 lux
 * ============================================================ */
#ifndef BH1750_H
#define BH1750_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* I2C configuration */
#define BH1750_I2C_HANDLE       hi2c1
#define BH1750_I2C_ADDR         (0x23 << 1)  /* ADDR pin LOW  */
/* #define BH1750_I2C_ADDR      (0x5C << 1)  // ADDR pin HIGH */

/* BH1750 command codes */
#define BH1750_CMD_POWER_ON     0x01
#define BH1750_CMD_RESET        0x07
#define BH1750_CMD_CONT_H_RES   0x10  /* Continuous High-Res Mode (1 lux, 120ms) */
#define BH1750_CMD_CONT_H_RES2  0x11  /* Continuous High-Res Mode 2 (0.5 lux)    */
#define BH1750_CMD_CONT_L_RES   0x13  /* Continuous Low-Res Mode (4 lux, 16ms)   */
#define BH1750_CMD_ONE_H_RES    0x20  /* One-Time High-Res Mode  */

/* Measurement timeout */
#define BH1750_MEAS_TIMEOUT_MS  200U

/* Extern handle (defined in main.c) */
extern I2C_HandleTypeDef hi2c1;

/* Public API */
void  BH1750_Init(void);
float BH1750_ReadLux(void);    /* Returns lux value, -1.0f on error */

#ifdef __cplusplus
}
#endif

#endif /* BH1750_H */
