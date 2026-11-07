/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* ── Sensor GPIO defines ────────────────────────────────── */
/* MAX31865 SPI4 CS — PE4 */
#define MAX31865_CS_Pin          GPIO_PIN_4
#define MAX31865_CS_Port         GPIOE
/* HC-SR04 TRIG — PF6, ECHO — PF7 (TIM2 CH1) */
#define HCSR04_TRIG_Pin          GPIO_PIN_6
#define HCSR04_TRIG_Port         GPIOF
#define HCSR04_ECHO_Pin          GPIO_PIN_7
#define HCSR04_ECHO_Port         GPIOF

/* ── Peripheral handle externs ──────────────────────────── */
extern SPI_HandleTypeDef  hspi4;    /* MAX31865 temperature      */
extern I2C_HandleTypeDef  hi2c1;    /* BMP280 + BH1750           */
extern ADC_HandleTypeDef  hadc3;    /* ACS712 current + MQ2 gas  */
extern TIM_HandleTypeDef  htim2;    /* HC-SR04 echo timing       */
extern TIM_HandleTypeDef  htim3;    /* Encoder RPM               */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
