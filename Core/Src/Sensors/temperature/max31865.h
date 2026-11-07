/* ============================================================
 * @file    max31865.h
 * @brief   MAX31865 RTD-to-Digital Converter driver (PT100)
 *          Interface: SPI4 — PE2=SCK, PE5=MISO, PE6=MOSI, PE4=CS
 * ============================================================ */
#ifndef MAX31865_H
#define MAX31865_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"

/* Hardware configuration */
#define MAX31865_SPI_HANDLE     hspi4
#define MAX31865_CS_PORT        GPIOE
#define MAX31865_CS_PIN         GPIO_PIN_4

/* PT100 reference and nominal values */
#define PT100_R_NOMINAL         100.0f    /* Resistance at 0°C (Ω)   */
#define PT100_R_REFERENCE       430.0f    /* Reference resistor (Ω)  */

/* MAX31865 register map */
#define MAX31865_REG_CONFIG     0x00
#define MAX31865_REG_RTD_MSB    0x01
#define MAX31865_REG_FAULT      0x07

/* Config register bits */
#define MAX31865_CFG_VBIAS      0x80
#define MAX31865_CFG_AUTOCONV   0x40
#define MAX31865_CFG_3WIRE      0x10
#define MAX31865_CFG_FAULT_CLR  0x02

/* Public API */
void    MAX31865_Init(void);
float   MAX31865_ReadTemperature(void);
uint8_t MAX31865_IsOk(void);
uint8_t MAX31865_ReadFault(void);

/* Extern handle (defined in main.c) */
extern SPI_HandleTypeDef hspi4;

#ifdef __cplusplus
}
#endif

#endif /* MAX31865_H */
