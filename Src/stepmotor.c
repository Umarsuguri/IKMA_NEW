#include "stepmotor.h"
#include "sensore.h"
#include "main.h"
#include "stm32f4xx.h"                  // Device header

#include <stdlib.h>
#include <math.h>
uint32_t X,Y,Z,X0,Y0; //Абсолютные координаты
//временные переменных
	int maxmax,minmin;
	//конец временных

int Sol01_X,Sol01_Y; //Абсолютная координата 0 соленоида 1
int Sol02_X,Sol02_Y; //Абсолютная координата 0 соленоида 2
int Sol03_X,Sol03_Y; //Абсолютная координата 0 соленоида 3
int Reader_X,Reader_Y; //Абсолютная координата 0 ридера

int sol01_dx;
int sol01_dy;
int sol02_dx;
int sol02_dy;
int sol03_dx;
int sol03_dy;
//extern UART_HandleTypeDef huart4;
uint8_t sens[4];

int8_t dir_X,dir_Y,dir_Z; //Направление движения
uint32_t step_Pulse_Width = STEP_PULS_WIDTH-1;
uint32_t dir_Change_Time = DIR_CHANG_TIME-1;

/**********************************************************************************/
/*  Переменные для расчета временных задержек, обеспечивающих заданное ускорение	*/
/**********************************************************************************/
		uint16_t cnt;
		uint16_t step_Delay;
		/*************************************************/
		/*  Переменные для ускорения по осям X и Y			 */
		/*************************************************/
		uint8_t N = (MAX_PF - MIN_PF)/(ACSEL/(1000000/TIME));
		uint16_t dF;
		uint16_t delays[255][2];
		/*************************************************/
		/*  Переменные для ускорения по оси Z						 */
		/*************************************************/
		uint8_t N_Z = (MAX_PF_Z - MIN_PF_Z)/(ACSEL_Z/(1000000/TIME_Z));
		uint16_t dF_Z;
		uint16_t delays_Z[255][2];



extern ADC_HandleTypeDef hadc1;

/**********/
/*  Шаги	*/
/**********/
void step_X(uint16_t Delay_for_Step)
{
	D_X_PUL_GPIO_Port -> BSRR = D_X_PUL_Pin;
	HAL_Delay(step_Pulse_Width);
	D_X_PUL_GPIO_Port -> BSRR = D_X_PUL_Pin << 16U;
	HAL_Delay(Delay_for_Step);
	X+=dir_X;
}
void step_Y(uint16_t Delay_for_Step)
{
	D_Y_PUL_GPIO_Port -> BSRR = D_Y_PUL_Pin;
	HAL_Delay(step_Pulse_Width);
	D_Y_PUL_GPIO_Port -> BSRR = D_Y_PUL_Pin << 16U;
	HAL_Delay(Delay_for_Step);
	Y+=dir_Y;
}

void step_Z(uint16_t Delay_for_Step)
{
	D_Z_PUL_GPIO_Port -> BSRR = D_Z_PUL_Pin;
	HAL_Delay(step_Pulse_Width);
	D_Z_PUL_GPIO_Port -> BSRR = D_Z_PUL_Pin << 16U;
	HAL_Delay(Delay_for_Step);
	Z+=dir_Z;
}

/************************************/
/*  Установка направления движения	*/
/************************************/
void set_DIR_X_rear()
{
	D_X_DIR_GPIO_Port -> BSRR = D_X_DIR_Pin;
	HAL_Delay(dir_Change_Time);
	dir_X = -1;
}
void set_DIR_X_front()
{
	D_X_DIR_GPIO_Port -> BSRR = D_X_DIR_Pin << 16U;
	HAL_Delay(dir_Change_Time);
	dir_X = 1;
}
void set_DIR_Y_rear()
{
	D_Y_DIR_GPIO_Port -> BSRR = D_Y_DIR_Pin;
	HAL_Delay(dir_Change_Time);
	dir_Y = -1;
}

void set_DIR_Y_front()
{
	D_Y_DIR_GPIO_Port -> BSRR = D_Y_DIR_Pin << 16U;
	HAL_Delay(dir_Change_Time);
	dir_Y = 1;
}
void set_DIR_Z_rear()
{
	D_Z_DIR_GPIO_Port -> BSRR = D_Z_DIR_Pin;
	HAL_Delay(dir_Change_Time);
	dir_Z = -1;
}
void set_DIR_Z_front()
{
	D_Z_DIR_GPIO_Port -> BSRR = D_Z_DIR_Pin << 16U;
	HAL_Delay(dir_Change_Time);
	dir_Z = 1;
}
/******************************/
/*  Переход в заданную точку	*/
/******************************/
MOVMENT_STATE move_HEAD_TO( uint32_t mX, uint32_t mY)
{
	if ((mX<=X_MAX)&&(mY<=Y_MAX))
	{
		
		int32_t dx, dy;
		dx=mX-X;
		dy=mY-Y;
		move_HEAD_X(dx);
		move_HEAD_Y(dy);
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;

	
}
// перемещения

/**************************************/
/*  Переход в заданную точку по оси Z	*/
/**************************************/
MOVMENT_STATE move_HEAD_X_TO( uint32_t mX)
{
	if (mX<=X_MAX)
	{
		int32_t dx;
		dx=mX-X;		
		move_HEAD_Z(dx);
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
//

/**************************************/
/*  Переход в заданную точку по оси Z	*/
/**************************************/
MOVMENT_STATE move_HEAD_Y_TO( uint32_t mY)
{
	if (mY<=Y_MAX)
	{
		int32_t dy;
		dy=mY-Y;		
		move_HEAD_Y(dy);
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
//

/**************************************/
/*  Переход в заданную точку по оси Z	*/
/**************************************/
MOVMENT_STATE move_HEAD_Z_TO( uint32_t mZ)
{
	if (mZ<=Z_MAX)
	{
		int32_t dz;
		dz=mZ-Z;		
		move_HEAD_Z(dz);
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
// перемещения

/**************************/
/*  Перемещение по оси X	*/
/**************************/
MOVMENT_STATE move_HEAD_X(int32_t dx)
{
	uint32_t mX = X+dx;
	if (mX<=X_MAX)
	{
		if (dx < 0) set_DIR_X_rear(); else set_DIR_X_front(); 
		dx=abs(dx);
		int i=0,j=0;
		int32_t DX =dx;
		
		/* Разгон */
		
		while ((i<N)&&(DX>dx/2))
		{
			j=0;
			step_Delay = delays[i][0];
			cnt = delays[i][1];
			while ((j<cnt)&&(DX>dx/2))
			{
				if (X_NOT_IN_0) step_X(step_Delay); 
				DX--;
				//HAL_Delay(step_Delay);
				j++;
			}
			i++;
		}
		
		/* Движение с постоянной скоростью */
		
		if (i==N)
		{
			int xl = 2*DX-dx; //Расстояние которое должен проходить на постоянно скорости
			for (j=0; j<xl; j++)
			{
				if (X_NOT_IN_0) step_X(step_Delay); DX--;
			}
			j=0;
		}
		
		/* Торможение */
		
		while ((i>=0)&&(DX>=0))
		{
			i--;
			step_Delay = delays[i][0];
			if (j!=0) { cnt = j; j=0;}
			else cnt = delays[i][1];
			while ((j<cnt)&&(DX>0))
			{
				if (DX>0) {if (X_NOT_IN_0) step_X(step_Delay); DX--;}
				j++;
			}
			j=0;
		}
		X=mX;
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
//

/**************************/
/*  Перемещение по оси Y	*/
/**************************/
MOVMENT_STATE move_HEAD_Y(int32_t dy)
{
	uint32_t mY = Y+dy;
	if (mY<=Y_MAX)
	{
		if (dy < 0) set_DIR_Y_rear(); else set_DIR_Y_front();
		dy=abs(dy);
		int i=0,j=0;
		int32_t DY =dy;
		
		/* Разгон */
		
		while ((i<N)&&(DY>dy/2))
		{
			j=0;
			step_Delay = delays[i][0];
			cnt = delays[i][1];
			while ((j<cnt)&&(DY>dy/2))
			{
				if (Y_NOT_IN_0) step_Y(step_Delay); 
				DY--;
				j++;
			}
			i++;
		}
		
		/* Движение с постоянной скоростью */
		
		if (i==N)
		{
			int yl = 2*DY-dy; //Расстояние которое должен проходить на постоянно скорости
			for (j=0; j<yl; j++)
			{
				if (Y_NOT_IN_0) step_Y(step_Delay); DY--;
			}
			j=0;
		}
		
		/* Торможение */
		
		while ((i>=0)&&(DY>=0))
		{
			i--;
			step_Delay = delays[i][0];
			if (j!=0) { cnt = j; j=0;}
			else cnt = delays[i][1];
			while ((j<cnt)&&(DY>0))
			{
				if (DY>0) {if (Y_NOT_IN_0) step_Y(step_Delay); DY--;}
				j++;
			}
			j=0;
		}  
		Y=mY;
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
//

/**************************/
/*  Перемещение по оси Z	*/
/**************************/
MOVMENT_STATE move_HEAD_Z(int32_t dz)
{
	uint32_t mZ = Z+dz;
	if (mZ<=Z_MAX)
	{
		if (dz < 0) set_DIR_Z_rear(); else set_DIR_Z_front();
		dz=abs(dz);
		int i=0,j=0;
		int32_t DZ =dz;
		
		/* Разгон */
		
		while ((i<N)&&(DZ>dz/2))
		{
			j=0;
			step_Delay = delays[i][0];
			cnt = delays[i][1];
			while ((j<cnt)&&(DZ>dz/2))
			{
				if (Z_NOT_IN_0) step_Z(step_Delay); 
				DZ--;
				j++;
			}
			i++;
		}
		
		/* Движение с постоянной скоростью */
		
		if (i==N)
		{
			int zl = 2*DZ-dz; //Расстояние которое должен проходить на постоянно скорости
			for (j=0; j<zl; j++)
			{
				if (Z_NOT_IN_0) step_Z(step_Delay); DZ--;
			}
			j=0;
		}
		
		/* Торможение */
		
		while ((i>=0)&&(DZ>=0))
		{
			i--;
			step_Delay = delays[i][0];
			if (j!=0) { cnt = j; j=0;}
			else cnt = delays[i][1];
			
			while ((j<cnt)&&(DZ>0))
			{
				if (DZ>0) {if (Z_NOT_IN_0) step_Z(step_Delay); DZ--;}
				j++;
			}
			j=0;
		} 
		Z=mZ;
		return MOVMENT_OK;
	}
	else return MOVMENT_ERR;
}
//

/**************************************************/
/*  Поиск трубы																		*/
/**************************************************/
MOVMENT_STATE search_PIPE(void)
{
	uint16_t meas,meas0;
	uint32_t MIN_Y,MAX_Y;
	
	/* Движение по оси Z пока не наткнемся на трубу. */
	
	/*  Разгон	*/
	
	set_DIR_Z_front();
	uint8_t i=0,j=0;
	while ((OBJ_S)&&(i<N_Z))
	{
		j=0;
		step_Delay=delays_Z[i][0];
		cnt = delays_Z[i][1];
		while ((OBJ_S)&&(j<cnt))
		{
			step_Z(step_Delay); 
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/
	
	if (i==N_Z)
	{
		step_Delay=delays_Z[N_Z-1][0];
		while(OBJ_S)
		{
			step_Z(step_Delay);
		}
	}
	
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays_Z[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays_Z[i][1];
		
		while (j<cnt)
		{
			step_Z(step_Delay);
			j++;
		}
		j=0;
	}
	
	
	/*  Выход из зоны датчика обнаружениЯ боковой поверхности трубы 	*/
//	
//	set_DIR_Z_rear();
//	step_Delay = delays_Z[0][0];
//	while(!(OBJ_S))
//	{
//		step_Z(step_Delay);
//	}
//	set_DIR_Z_front();
	
	
	
	// Поиск кромки
	meas = get_OBJ_MEAS_Data();
	int MAX_Meas = 3615;
	meas0 = meas;
	
	/*  Разгон	*/
	set_DIR_Y_front();
	while ((Y<Y_MAX)&&(i<N))
	{
		j=0;
		step_Delay=delays[i][0];
		cnt = delays[i][1]+1;
		while ((Y<Y_MAX)&&(j<cnt))
		{
			step_Y(step_Delay); 
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/	
	
	if (i==N)
	{
		step_Delay=delays[N-1][0];
		
		/* 		Поиск нижней точки		*/
		
		while((Y<=Y_MAX)&((MAX_Meas - meas)<500))
		{
			for(int l=0; l<160; l++)
			{
				step_Y(step_Delay);
			}
			meas = get_OBJ_MEAS_Data();
			if (abs(meas0-meas)>150) meas =meas0;
			else meas0=meas;
		}
		MIN_Y = Y;
		minmin=Y;
		
		/* движение от нижней точки (для избежания ошибочного нахождения верхней точки )*/
		
		for(int l=0; l<3200; l++)
		{
			step_Y(step_Delay);
		}
			meas = get_OBJ_MEAS_Data();
			meas0 = meas;
		
		/* 		Поиск верхней точки		*/
		
		while((Y<=Y_MAX)&((MAX_Meas - meas)>500))
		{
			for(int l=0; l<160; l++)
			{
				step_Y(step_Delay);
			}
			meas = get_OBJ_MEAS_Data();
			if (abs(meas0-meas)>150) meas =meas0;
			else meas0=meas;
		}
		MAX_Y = Y;
		
		maxmax=Y;
	}
	
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays[i][1];
		
		while (j<cnt)
		{
			step_Y(step_Delay);
			j++;
		}
		j=0;
	}
	
	/* Движение в середину трубы */
	
	move_HEAD_TO(0,(MAX_Y + MIN_Y)/2);
	
	/* Установка на заданное расстоянии*/
	if (meas > S)
	{
		set_DIR_Z_front();
		while(meas > S)
		{
			step_Z(500);
			meas = get_OBJ_MEAS_Data();
		}
	}
	else 
	{
		set_DIR_Z_rear();
		while(meas <= S)
		{
			step_Z(500);
			meas = get_OBJ_MEAS_Data();
		}
	}
	
	
	Sol01_X = X+SOL1DX;
	Sol01_Y = Y+SOL1DY;
	Sol02_X = X+SOL2DX+sol02_dx;
	Sol02_Y = Y+SOL2DY+sol02_dy;
	Sol03_X = X+SOL3DX;
	Sol03_Y = Y+SOL3DY;
	Reader_X = X+READDX;
	Reader_Y = Y+READDY;
	move_HEAD_TO(Sol01_X,Sol01_Y);
	return MOVMENT_OK;
}

//

/**************************************************/
/*  Возврат машины в исходное состояние по оси X	*/
/**************************************************/
MOVMENT_STATE go_HOME_X(void)
{
	set_DIR_X_rear();
	uint8_t i=0,j=0;
	
	/*  Разгон	*/
	
	while ((X_NOT_IN_0)&&(i<N))
	{
		j=0;
		step_Delay = delays[i][0];
		cnt = delays[i][1];
		while ((X_NOT_IN_0)&&(j<cnt))
		{
			step_X(step_Delay);
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/	
	
	if (i==N)
	{
		step_Delay=delays[N-1][0];
		while(X_NOT_IN_0)
		{
			step_X(step_Delay);
		}
	}
	
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays[i][1];
		
		while (j<cnt)
		{
			step_X(step_Delay);
			j++;
		}
		j=0;
	}
	
	/*  Выход из зоны концевика	*/
	
	set_DIR_X_front();
	step_Delay = delays[0][0];
	while(!(X_NOT_IN_0))
	{
		step_X(step_Delay);
	}
	X=0;
	move_HEAD_X(3200);
	X=0;
}

//

/**************************************************/
/*  Возврат машины в исходное состояние по оси Y	*/
/**************************************************/
MOVMENT_STATE go_HOME_Y(void)
{
	set_DIR_Y_rear();
	uint8_t i=0,j=0;
	
	/*  Разгон	*/
	
	while ((Y_NOT_IN_0)&&(i<N))
	{
		j=0;
		step_Delay=delays[i][0];
		cnt = delays[i][1]+1;
		while ((Y_NOT_IN_0)&&(j<cnt))
		{
			step_Y(step_Delay);
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/	
	
	if (i==N)
	{
		step_Delay=delays[N-1][0];
		
		while(Y_NOT_IN_0)
		{
			step_Y(step_Delay);
		}
	}
	
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays[i][1];
		
		while (j<cnt)
		{
			step_Y(step_Delay);
			j++;
		}
		j=0;
	}
	
	/*  Выход из зоны концевика	*/
	
	set_DIR_Y_front();
	step_Delay = delays[0][0];
	while(!(Y_NOT_IN_0))
	{
		step_Y(step_Delay);
	}
	Y=0;
	move_HEAD_Y(3200);
	Y=0;
}

//

/**************************************************/
/*  Возврат машины в исходное состояние по оси Z	*/
/**************************************************/
MOVMENT_STATE go_HOME_Z(void)
{
	set_DIR_Z_rear();
	uint8_t i=0,j=0;
	
	/*  Разгон	*/
	
	while ((Z_NOT_IN_0)&&(i<N_Z))
	{
		j=0;
		step_Delay=delays_Z[i][0];
		cnt = delays_Z[i][1]+1;
		while ((Z_NOT_IN_0)&&(j<cnt))
		{
			step_Z(step_Delay);
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/
	
	if (i==N_Z)
	{
		step_Delay=delays_Z[N_Z-1][0];
		while(Z_NOT_IN_0)
		{
			step_Z(step_Delay);
		}
	}
		
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays_Z[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays_Z[i][1];
		
		while (j<cnt)
		{
			step_Z(step_Delay);
			j++;
		}
		j=0;
	}
			
	/*  Выход из зоны концевика	*/
	
	set_DIR_Z_front();
	step_Delay = delays_Z[0][0];
	while(!(Z_NOT_IN_0))
	{
		step_Z(step_Delay);
	}
	Z=0;
	move_HEAD_Z(2710);
	Z=0;
	
}

//

/******************************************************/
/*  Возврат машины в исходное состояние по всем осям	*/
/******************************************************/
MOVMENT_STATE go_HOME(void)
{
	go_HOME_Z();	
	go_HOME_X();
	go_HOME_Y();
}

//


/**************************************************/
/*  Переход в нулевую точку иглоудраной головки		*/
/**************************************************/
MOVMENT_STATE go_TO_SOL01_0(void)
{
	return move_HEAD_TO(Sol01_X,Sol01_Y); 
}

//

/**************************************************/
/*  Переход в нулевую точку каплеструйной головки	*/
/**************************************************/
MOVMENT_STATE go_TO_SOL02_0(void)
{
	return move_HEAD_TO(Sol02_X,Sol02_Y);
}

//

/******************************************************************/
/*  Расчет временных задержек, обеспечивающих заданное ускорение	*/
/******************************************************************/
void calc_Delays(void)
{
	dF = (MAX_PF-MIN_PF)/N;
	for (int i=0; i<N; i++)
	{
		delays[i][0] = 1000000/(MIN_PF+dF*i)-STEP_PULS_WIDTH-1;
		delays[i][1] = TIME / delays[i][0];
	}
	
	dF_Z = (MAX_PF_Z-MIN_PF_Z)/N_Z;
	for (int i=0; i<N_Z; i++)
	{
		delays_Z[i][0] = 1000000/(MIN_PF_Z+dF_Z*i)-STEP_PULS_WIDTH-1;
		delays_Z[i][1] = TIME_Z / delays_Z[i][0];
	}
}

//

/********************************/
/*  Сканирования профиля трубы	*/
/********************************/
void scan_PIPE(void)
{
	int meas,meas0;
	uint32_t MIN_Y,MAX_Y;
	
	uint8_t i=0,j=0;
	// Поиск кромки
	meas = get_OBJ_MEAS_Data();
	int MAX_Meas = 3615;
	meas0 = meas;
	
	/*  Разгон	*/
	set_DIR_Y_front();
	while ((Y<Y_MAX)&&(i<N))
	{
		j=0;
		step_Delay=delays[i][0];
		cnt = delays[i][1]+1;
		while ((Y<Y_MAX)&&(j<cnt))
		{
			step_Y(step_Delay); 
			j++;
		}
		i++;
	}
	
	/*  Движение с постоянной скоростью	*/	
	
	if (i==N)
	{
		step_Delay=delays[N-1][0];
		
		/* 		Поиск нижней точки		*/
		
		while((Y<=Y_MAX)&((MAX_Meas - meas)<500))
		{
			for(int l=0; l<160; l++)
			{
				step_Y(step_Delay);
			}
			meas = get_OBJ_MEAS_Data();
			if (abs(meas0-meas)>150) meas =meas0;
			else meas0=meas;
		}
		MIN_Y = Y;
		/* движение от нижней точки (для избежания ошибочного нахождения верхней точки )*/
		
		for(int l=0; l<3200; l++)
		{
			step_Y(step_Delay);
		}
			meas = get_OBJ_MEAS_Data();
			meas0 = meas;
		
		/* 		Поиск верхней точки		*/
		
		while((Y<=Y_MAX)&((MAX_Meas - meas)>500))
		{
			for(int l=0; l<160; l++)
			{
				step_Y(step_Delay);
			}
			meas = get_OBJ_MEAS_Data();
			if (abs(meas0-meas)>150) meas =meas0;
			else meas0=meas;
		}
		MAX_Y = Y;
	}
	
	/*  Торможение	*/
	
	while (i>0)
	{
		i--;
		step_Delay = delays[i][0];
		if (j!=0) { cnt = j; j=0;}
		else cnt = delays[i][1];
		
		while (j<cnt)
		{
			step_Y(step_Delay);
			j++;
		}
		j=0;
	}
	
	/* Движение в середину трубы */
	
	move_HEAD_TO(0,(MAX_Y + MIN_Y)/2);
}

//
