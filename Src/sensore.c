#include "sensore.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart4;

void read_sensor_data(uint8_t sens[16])
{
	uint16_t meas = get_OBJ_MEAS_Data();
	sens[0] = 0xff;
	sens[1] = 0x01;
		
	if (HAL_GPIO_ReadPin(D_Z_0_GPIO_Port,D_Z_0_Pin) == GPIO_PIN_SET)						{sens[2] = 1;}		else {sens[2] = 0;}
	if (HAL_GPIO_ReadPin(D_Z_ALM_GPIO_Port,D_Z_ALM_Pin)== GPIO_PIN_SET)					{sens[3] = 1;}		else {sens[3] = 0;}
	if (HAL_GPIO_ReadPin(D_X_0_GPIO_Port,D_X_0_Pin)== GPIO_PIN_SET)							{sens[4] = 1;}		else {sens[4] = 0;}
	if (HAL_GPIO_ReadPin(D_X_ALM_GPIO_Port,D_X_ALM_Pin)== GPIO_PIN_SET)					{sens[5] = 1;}		else {sens[5] = 0;}
	if (HAL_GPIO_ReadPin(D_Y_0_GPIO_Port,D_Y_0_Pin)== GPIO_PIN_SET)							{sens[6] = 1;}		else {sens[6] = 0;}
	if (HAL_GPIO_ReadPin(D_Y_ALM_GPIO_Port,D_Y_ALM_Pin)== GPIO_PIN_SET)					{sens[7] = 1;}		else {sens[7] = 0;}
																																							 sens[8] = meas;
																																							 sens[9] = meas>>8;
	if (HAL_GPIO_ReadPin(D_IN01_GPIO_Port,D_IN01_Pin) == GPIO_PIN_SET) 					{sens[10] = 1;}		else {sens[10] = 0;}
	if (HAL_GPIO_ReadPin(D_IN02_GPIO_Port,D_IN02_Pin) == GPIO_PIN_SET) 					{sens[11] = 1;}		else {sens[11] = 0;}
	if (HAL_GPIO_ReadPin(D_IN03_GPIO_Port,D_IN03_Pin) == GPIO_PIN_SET)					{sens[12] = 1;}		else {sens[12] = 0;}
	if (HAL_GPIO_ReadPin(D_IN04_GPIO_Port,D_IN04_Pin) == GPIO_PIN_SET) 					{sens[13] = 1;}		else {sens[13] = 0;}
	if (HAL_GPIO_ReadPin(D_OBJ_IN_GPIO_Port,D_OBJ_IN_Pin) == GPIO_PIN_SET) 			{sens[14] = 1;}		else {sens[14] = 0;}
	if (HAL_GPIO_ReadPin(D_OBJ_OUT_GPIO_Port,D_OBJ_OUT_Pin) == GPIO_PIN_SET)		{sens[15] = 1;}		else {sens[15] = 0;}
	if (HAL_GPIO_ReadPin(D_OBJ_BUTT_GPIO_Port,D_OBJ_BUTT_Pin) == GPIO_PIN_SET)	{sens[16] = 1;}		else {sens[16] = 0;}
	if (HAL_GPIO_ReadPin(D_OBJ_S_GPIO_Port,D_OBJ_S_Pin) == GPIO_PIN_SET) 				{sens[17] = 1;}		else {sens[17] = 0;}
	if (HAL_GPIO_ReadPin(D_DISP_P0_GPIO_Port,D_DISP_P0_Pin) == GPIO_PIN_SET)		{sens[18] = 1;}		else {sens[18] = 0;}
	if (HAL_GPIO_ReadPin(D_DISP_P1_GPIO_Port,D_DISP_P1_Pin) == GPIO_PIN_SET) 		{sens[19] = 1;}		else {sens[19] = 0;}
	
	HAL_UART_Transmit(&huart4,sens,20,100000);
}

uint16_t get_OBJ_MEAS_Data(void)
{
	uint16_t value;
	HAL_ADC_Start(&hadc1);
	while(HAL_ADC_PollForConversion(&hadc1,10000));
	value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return value;
}
