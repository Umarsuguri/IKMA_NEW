#include "stm32f4xx_hal.h"





typedef enum  
{
	SOL_PW_1 = 1,
	SOL_PW_2 = 2,
	SOL_PW_3 = 3,
	SOL_PW_4 = 4,
	SOL_PW_5 = 5,
	SOL_PW_6 = 6,
	SOL_PW_7 = 7,
	SOL_PW_8 = 8,
	SOL_PW_9 = 9
}SOL_PW;

/* Сила удара */
extern SOL_PW SOL01_PW;
extern SOL_PW SOL02_PW;

/* Размера капли */

void Sol01_stike(SOL_PW Strike_power);
void Sol02_stike(SOL_PW Strike_power);

