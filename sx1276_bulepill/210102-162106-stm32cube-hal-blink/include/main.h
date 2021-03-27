/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define SD_CS_Pin GPIO_PIN_12
#define SD_CS_GPIO_Port GPIOB
#define SD_SPI_HANDLE hspi2

#define MODE_Pin GPIO_PIN_13
#define MODE_GPIO_Port GPIOC

#define NSS_Pin GPIO_PIN_12  //NSS
#define NSS_GPIO_Port GPIOB

#define LED_Pin GPIO_PIN_13 //SCK2
#define LED_GPIO_Port GPIOC

#define DIO0_Pin GPIO_PIN_0
#define DIO0_GPIO_Port GPIOB


#define RESET_Pin GPIO_PIN_1
#define RESET_GPIO_Port GPIOB

#define DEBUG 0

#ifdef __cplusplus
}
#endif

typedef enum {SDNoInitialized = 0, SDOk} SDStatus;



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
