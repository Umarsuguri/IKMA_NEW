#include "solenoids.h"
#include "sensore.h"
#include "main.h"
#include "stm32f4xx.h"                  // Device header

#include <stdlib.h>
#include <math.h>

SOL_PW SOL01_PW;
SOL_PW SOL02_PW;

void Sol01_stike(SOL_PW Strike_power)
{
	int c;
	HAL_GPIO_WritePin(D_SOL01_EN_GPIO_Port,D_SOL01_EN_Pin,GPIO_PIN_SET);
	
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_SET);
		HAL_Delay(1050);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_RESET);
	
		HAL_Delay(75);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_SET);	
		HAL_Delay(39);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_RESET);
		HAL_Delay(75);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_SET);	
		HAL_Delay(39);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_RESET);
		HAL_Delay(75);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_SET);	
		HAL_Delay(39);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_RESET);
	
	c=(Strike_power-1)*5;
	
	for(int i=0; i<c; i++)
	{
		HAL_Delay(75);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_SET);	
		HAL_Delay(39);
		HAL_GPIO_WritePin(D_SOL01_GPIO_Port,D_SOL01_Pin,GPIO_PIN_RESET);
		
	}
	HAL_GPIO_WritePin(D_SOL01_EN_GPIO_Port,D_SOL01_EN_Pin,GPIO_PIN_RESET);
	HAL_Delay(6000);
}

void Sol02_stike(SOL_PW Strike_power)
{
	HAL_Delay(5000);
	HAL_GPIO_WritePin(D_SOL02_EN_GPIO_Port,D_SOL02_EN_Pin,GPIO_PIN_SET);
	HAL_Delay(20);
	
		HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_SET);
		HAL_Delay(140);
		HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_RESET);
	
	for (int j=1; j<Strike_power; j++)
	{
			HAL_Delay(15);
			HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_SET);
			HAL_Delay(5);
			HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(D_SOL02_EN_GPIO_Port,D_SOL02_EN_Pin,GPIO_PIN_RESET);
	HAL_Delay(5000);
}
