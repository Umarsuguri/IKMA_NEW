#include "stm32f4xx_hal.h"

#define Z_NOT_IN_0 		HAL_GPIO_ReadPin(D_Z_0_GPIO_Port,D_Z_0_Pin)						!= GPIO_PIN_SET				
#define Z_ALM 				HAL_GPIO_ReadPin(D_Z_ALM_GPIO_Port,D_Z_ALM_Pin)				== GPIO_PIN_SET		
#define X_NOT_IN_0		HAL_GPIO_ReadPin(D_X_0_GPIO_Port,D_X_0_Pin)						!= GPIO_PIN_SET
#define X_ALM					HAL_GPIO_ReadPin(D_X_ALM_GPIO_Port,D_X_ALM_Pin)				== GPIO_PIN_SET
#define Y_NOT_IN_0		HAL_GPIO_ReadPin(D_Y_0_GPIO_Port,D_Y_0_Pin)						!= GPIO_PIN_SET
#define Y_ALM					HAL_GPIO_ReadPin(D_Y_ALM_GPIO_Port,D_Y_ALM_Pin)				== GPIO_PIN_SET

#define OBJ_MEAS			HAL_ADC_GetValue(&hadc1)																															 

#define IN1						HAL_GPIO_ReadPin(D_IN01_GPIO_Port,D_IN01_Pin)					== GPIO_PIN_SET
#define IN2						HAL_GPIO_ReadPin(D_IN02_GPIO_Port,D_IN02_Pin)					== GPIO_PIN_SET
#define IN3						HAL_GPIO_ReadPin(D_IN03_GPIO_Port,D_IN03_Pin)					== GPIO_PIN_SET
#define IN4						HAL_GPIO_ReadPin(D_IN04_GPIO_Port,D_IN04_Pin)					== GPIO_PIN_SET

#define OBJ_IN				HAL_GPIO_ReadPin(D_OBJ_IN_GPIO_Port,D_OBJ_IN_Pin)			== GPIO_PIN_SET
#define OBJ_OUT				HAL_GPIO_ReadPin(D_OBJ_OUT_GPIO_Port,D_OBJ_OUT_Pin)		== GPIO_PIN_SET
#define OBJ_BUTT			HAL_GPIO_ReadPin(D_OBJ_BUTT_GPIO_Port,D_OBJ_BUTT_Pin)	== GPIO_PIN_SET
#define OBJ_S					HAL_GPIO_ReadPin(D_OBJ_S_GPIO_Port,D_OBJ_S_Pin)				!= GPIO_PIN_SET

#define DISP_P0				HAL_GPIO_ReadPin(D_DISP_P0_GPIO_Port,D_DISP_P0_Pin)		== GPIO_PIN_SET
#define DISP_P1				HAL_GPIO_ReadPin(D_DISP_P1_GPIO_Port,D_DISP_P1_Pin)		== GPIO_PIN_SET



void read_sensor_data(uint8_t sens[16]);
uint16_t get_OBJ_MEAS_Data(void);
