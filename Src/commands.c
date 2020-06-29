/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "sensore.h"
#include "stepmotor.h"
#include "solenoids.h"
#include "commands.h"
#include <math.h>


uint8_t cmd_rc,cmd_er;
uint8_t cmd[CMD_LENGHT];
uint8_t cs_ERRORE[5]= {0xff,0x0f,0x01,0x01,0x10};
uint8_t cmd_RX[1];
uint8_t cnt_RX;
uint8_t cnt_Mrk=0;
uint8_t cnt_Point=0;

uint8_t washing_stat=0; // режим промывки каплеструйной головки
uint8_t pipe_scaning_stat=0; // режим сканирования профиля трубы и поиска середины

uint16_t move_X[255];
uint16_t move_Y[255];
uint16_t k;

uint8_t cs;

uint8_t data[5];

void cmd_do(void)
{
	if (cmd_er == 1) 
	{
		HAL_UART_Transmit(&huart4, cs_ERRORE, CMD_LENGHT,100000);
		for (int i = 0; i<CMD_LENGHT; i++) cmd[i] = 0;
		cmd_er =0;
	}
	if (cmd_rc == 1) 
	{
			// 	Обработка стандартных команд
			if (cmd[1] == CMD_STD) 
			{
				// Промывка форсунки Вкл/Выкл
				
				if (cmd[2] == WHASH) 
				{
					if (cmd[3] == WHASH_START) washing_stat = 1;
					if (cmd[3] == WHASH_STOP) washing_stat = 0;
				}
				
				//Возврат каретки
				if (cmd[2] == GO_HOME) 
					go_HOME();
				
				//Старт маркирования
				
				if (cmd[2] == START_MARKING) 
				{
					search_PIPE();
					/* Иглоударка */
					Sol01_X=Sol01_X+cnt_Mrk*25*step_X_MM;
					Sol02_X=Sol02_X+cnt_Mrk*25*step_X_MM;
					
					move_HEAD_TO(Sol01_X,Sol01_Y);
					for (int i=0;i<k;i++)
					{
						move_HEAD_TO(Sol01_X+move_X[i],Sol01_Y+move_Y[i]);
						Sol01_stike(SOL01_PW);
					}
					/* Каплеструйка */
					Sol02_stike(SOL02_PW);
					Sol02_stike(SOL02_PW);
					move_HEAD_TO(Sol02_X,Sol02_Y);
					for (int i=0;i<k;i++)
					{
						move_HEAD_TO(Sol02_X+move_X[i],Sol02_Y+move_Y[i]);
						Sol02_stike(SOL02_PW);
					}
					/* Ридер */
					move_HEAD_TO(Reader_X,Reader_Y);
					HAL_Delay(1000000);
					move_HEAD_Z_TO(0);
					move_HEAD_TO(0,0);
					
					cnt_Mrk++;
					if (cnt_Mrk>=6) cnt_Mrk=0;
				}
				//
				
				
			}
			
			// 	Обработка конфигурационных команд
			if (cmd[1]	==	CMD_CFG)
			{
				/*настройка длительностей импульсов*/
				if (cmd[2]	==	SET_SOL01_PW) 
					SOL01_PW = cmd[3];
				if (cmd[2]	==	SET_SOL02_PW) 
					SOL02_PW = cmd[3];
				
				/*юстировка головок*/
				if (cmd[2]	==	SET_CALL_PAR_X) sol02_dx = (cmd[3]-127)*step_X_MM/10; 
				if (cmd[2]	==	SET_CALL_PAR_Y) sol02_dy = (cmd[3]-127)*step_Y_MM/10;
			}
			
		
			
			// 	Обработка тестовых команд
			if (cmd[1]	==	CMD_TST) 
			{
				// Тесто поиска середины трубы
				if (cmd[2]	==	SEARCH_PIPE) search_PIPE();
				// Тестовый плевок форсункой
				if (cmd[2]	==	STRIKE_SOL02) Sol02_stike(cmd[3]);
				
				// Тестовый удар иглоударкой
				if (cmd[2]	==	STRIKE_SOL01) Sol01_stike(cmd[3]);
				
				//Поставить точку
				if (cmd[2] == MARK_POINT) 
				{
					/* Иглоударка */
					move_HEAD_TO(Sol01_X,Sol01_Y);
					Sol01_stike(SOL01_PW);
					/* Каплеструйка */
					move_HEAD_TO(Sol02_X,Sol02_Y);
					Sol02_stike(SOL02_PW);
				}
				
				//Режим сканирования трубы ВКЛ/ВЫКЛ
				if (cmd[2]	==	SCAN_PIPE_LT) 
				{
					if (cmd[3] == SCAN_PIPE_START)	
					{
						search_PIPE();
						move_HEAD_Y_TO(0);
						pipe_scaning_stat = 1;	//ВКЛ
					}
					if (cmd[3] == SCAN_PIPE_STOP) 	pipe_scaning_stat = 0;	//ВЫКЛ
				}
			}
		
		HAL_UART_Transmit(&huart4, cmd, CMD_LENGHT,100000);
		for (int i = 0; i<CMD_LENGHT; i++) cmd[i] = 0;
		cmd_rc = 0;
		
	}
	
	if (washing_stat ==1) 
	{
		HAL_GPIO_WritePin(D_SOL02_EN_GPIO_Port,D_SOL02_EN_Pin,GPIO_PIN_SET);
		HAL_Delay(20);
	
		HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_SET);
		HAL_Delay(180);
		HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_RESET);
	
		for (int j=1; j<10000; j++)
		{
				HAL_Delay(15);
				HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_SET);
				HAL_Delay(5);
				HAL_GPIO_WritePin(D_SOL02_GPIO_Port,D_SOL02_Pin,GPIO_PIN_RESET);
		}
		HAL_GPIO_WritePin(D_SOL02_EN_GPIO_Port,D_SOL02_EN_Pin,GPIO_PIN_RESET);
		
		HAL_Delay(50000);
		HAL_Delay(50000);
		HAL_Delay(20000);
	}
	if (pipe_scaning_stat ==1) 
	{
		scan_PIPE();
		data[0] = 0xff;
		data[1] = 0x11;
		data[2] = Y>>8;
		data[3] = Y;
		data[4] = data[0] + data[1] + data[2] + data[3];
		HAL_UART_Transmit(&huart4, data, 5,100000);
		move_HEAD_Y_TO(0);
	}
	
}
