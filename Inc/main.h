/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define D_OUT04_Pin GPIO_PIN_13
#define D_OUT04_GPIO_Port GPIOC
#define D_OBJ_IN_Pin GPIO_PIN_0
#define D_OBJ_IN_GPIO_Port GPIOC
#define D_OBJ_OUT_Pin GPIO_PIN_1
#define D_OBJ_OUT_GPIO_Port GPIOC
#define D_OBJ_BUTT_Pin GPIO_PIN_2
#define D_OBJ_BUTT_GPIO_Port GPIOC
#define D_OBJ_S_Pin GPIO_PIN_3
#define D_OBJ_S_GPIO_Port GPIOC
#define D_Tx_PLC_Pin GPIO_PIN_0
#define D_Tx_PLC_GPIO_Port GPIOA
#define D_Rx_PLC_Pin GPIO_PIN_1
#define D_Rx_PLC_GPIO_Port GPIOA
#define D_Tx_S_Pin GPIO_PIN_2
#define D_Tx_S_GPIO_Port GPIOA
#define D_Rx_S_Pin GPIO_PIN_3
#define D_Rx_S_GPIO_Port GPIOA
#define D_Z_PUL_Pin GPIO_PIN_4
#define D_Z_PUL_GPIO_Port GPIOA
#define D_Z_DIR_Pin GPIO_PIN_5
#define D_Z_DIR_GPIO_Port GPIOA
#define D_X_PUL_Pin GPIO_PIN_6
#define D_X_PUL_GPIO_Port GPIOA
#define D_X_DIR_Pin GPIO_PIN_7
#define D_X_DIR_GPIO_Port GPIOA
#define D_DISP_P0_Pin GPIO_PIN_4
#define D_DISP_P0_GPIO_Port GPIOC
#define D_DISP_P1_Pin GPIO_PIN_5
#define D_DISP_P1_GPIO_Port GPIOC
#define D_OBJ_MEAS_Pin GPIO_PIN_0
#define D_OBJ_MEAS_GPIO_Port GPIOB
#define D_Z_0_Pin GPIO_PIN_10
#define D_Z_0_GPIO_Port GPIOB
#define D_Z_ALM_Pin GPIO_PIN_11
#define D_Z_ALM_GPIO_Port GPIOB
#define D_Y_0_Pin GPIO_PIN_12
#define D_Y_0_GPIO_Port GPIOB
#define D_Y_ALM_Pin GPIO_PIN_13
#define D_Y_ALM_GPIO_Port GPIOB
#define D_X_0_Pin GPIO_PIN_14
#define D_X_0_GPIO_Port GPIOB
#define D_X_ALM_Pin GPIO_PIN_15
#define D_X_ALM_GPIO_Port GPIOB
#define D_SOL07_Pin GPIO_PIN_8
#define D_SOL07_GPIO_Port GPIOD
#define D_SOL08_Pin GPIO_PIN_9
#define D_SOL08_GPIO_Port GPIOD
#define D_SOL09_Pin GPIO_PIN_10
#define D_SOL09_GPIO_Port GPIOD
#define D_SOL10_Pin GPIO_PIN_11
#define D_SOL10_GPIO_Port GPIOD
#define D_SOL11_Pin GPIO_PIN_12
#define D_SOL11_GPIO_Port GPIOD
#define D_IN01_Pin GPIO_PIN_6
#define D_IN01_GPIO_Port GPIOC
#define D_IN02_Pin GPIO_PIN_7
#define D_IN02_GPIO_Port GPIOC
#define D_IN03_Pin GPIO_PIN_8
#define D_IN03_GPIO_Port GPIOC
#define D_IN04_Pin GPIO_PIN_9
#define D_IN04_GPIO_Port GPIOC
#define D_Y_PUL_Pin GPIO_PIN_8
#define D_Y_PUL_GPIO_Port GPIOA
#define D_Y_DIR_Pin GPIO_PIN_9
#define D_Y_DIR_GPIO_Port GPIOA
#define D_OUT01_Pin GPIO_PIN_10
#define D_OUT01_GPIO_Port GPIOC
#define D_OUT02_Pin GPIO_PIN_11
#define D_OUT02_GPIO_Port GPIOC
#define D_OUT03_Pin GPIO_PIN_12
#define D_OUT03_GPIO_Port GPIOC
#define D_SOL01_EN_Pin GPIO_PIN_0
#define D_SOL01_EN_GPIO_Port GPIOD
#define D_SOL01_Pin GPIO_PIN_1
#define D_SOL01_GPIO_Port GPIOD
#define D_SOL02_EN_Pin GPIO_PIN_2
#define D_SOL02_EN_GPIO_Port GPIOD
#define D_SOL02_Pin GPIO_PIN_3
#define D_SOL02_GPIO_Port GPIOD
#define D_SOL03_Pin GPIO_PIN_4
#define D_SOL03_GPIO_Port GPIOD
#define D_SOL04_Pin GPIO_PIN_5
#define D_SOL04_GPIO_Port GPIOD
#define D_SOL05_Pin GPIO_PIN_6
#define D_SOL05_GPIO_Port GPIOD
#define D_SOL06_Pin GPIO_PIN_7
#define D_SOL06_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
