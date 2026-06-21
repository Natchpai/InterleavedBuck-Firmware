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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_IDLE_Pin GPIO_PIN_0
#define LED_IDLE_GPIO_Port GPIOC
#define LED_RUN_Pin GPIO_PIN_1
#define LED_RUN_GPIO_Port GPIOC
#define LED_FAULT_Pin GPIO_PIN_2
#define LED_FAULT_GPIO_Port GPIOC
#define LED_USER1_Pin GPIO_PIN_3
#define LED_USER1_GPIO_Port GPIOC
#define ADC1_I_CH1_Pin GPIO_PIN_3
#define ADC1_I_CH1_GPIO_Port GPIOA
#define COMP2_I_CH1_Pin GPIO_PIN_7
#define COMP2_I_CH1_GPIO_Port GPIOA
#define ADC2_I_CH2_Pin GPIO_PIN_4
#define ADC2_I_CH2_GPIO_Port GPIOC
#define COMP4_I_CH2_Pin GPIO_PIN_0
#define COMP4_I_CH2_GPIO_Port GPIOB
#define ADC3_VOUT_Pin GPIO_PIN_1
#define ADC3_VOUT_GPIO_Port GPIOB
#define ADC4_IN3_VIN_Pin GPIO_PIN_12
#define ADC4_IN3_VIN_GPIO_Port GPIOB
#define ADC4_IN4_I_IN_Pin GPIO_PIN_14
#define ADC4_IN4_I_IN_GPIO_Port GPIOB
#define PWM_HS_CH2_Pin GPIO_PIN_6
#define PWM_HS_CH2_GPIO_Port GPIOC
#define PWM_LS_CH2_Pin GPIO_PIN_7
#define PWM_LS_CH2_GPIO_Port GPIOC
#define EN_CH2_Pin GPIO_PIN_9
#define EN_CH2_GPIO_Port GPIOC
#define PWM_HS_CH1_Pin GPIO_PIN_8
#define PWM_HS_CH1_GPIO_Port GPIOA
#define PWM_LS_CH1_Pin GPIO_PIN_9
#define PWM_LS_CH1_GPIO_Port GPIOA
#define EN_CH1_Pin GPIO_PIN_10
#define EN_CH1_GPIO_Port GPIOA
#define SW1_USER_Pin GPIO_PIN_15
#define SW1_USER_GPIO_Port GPIOA
#define SW1_USER_EXTI_IRQn EXTI15_10_IRQn
#define ISR_Pin GPIO_PIN_12
#define ISR_GPIO_Port GPIOC
#define Set_Duty_Pin GPIO_PIN_2
#define Set_Duty_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
