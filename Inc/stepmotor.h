#include "stm32f4xx_hal.h"

#define STEP_DELAY							250  		//������ ����
#define STEP_PULS_WIDTH					10			//������������ �������� ����
#define DIR_CHANG_TIME					1000		//������������ ��������� ����� ����� �����������

#define S 											1687		//��������� �� ����� �� ������������ ������� (������������) 241 ������� �� ���������

#define step_X_MM 							320 		//���������� ����� �� �� �� ��� X
#define step_Y_MM 							320			//���������� ����� �� �� �� ��� Y
#define step_Z_MM 							271			//���������� ����� �� �� �� ��� Z

#define X_MAX 									80000		
#define Y_MAX 									80000		
#define Z_MAX 									80000		

/************************************************/
/*  ��������� ��������� �� ���� X � Y			*/
/************************************************/
#define ACSEL 									700000			//�������� ���������� �������, 		|	��/�2	(1600 ����� �� ������, 5��)
#define	TIME										1000				//������ ����� ������, 						|	���
#define MIN_PF									1000				//��������� ������� ��� �������, 	|	��
#define MAX_PF									32000				//������������ ������� ���������, |	��

/************************************************/
/*  ��������� ��������� �� ��� Z				*/
/************************************************/
#define ACSEL_Z									300000			//�������� ���������� �������, 		|	��/�2 (25600 ����� �� ������, 94.248��)
#define	TIME_Z									1000				//������ ����� ������, 						|	���
#define MIN_PF_Z								1000				//��������� ������� ��� �������, 	|	��
#define MAX_PF_Z								25600				//������������ ������� ���������, |	��

/************************************************/
/*  ������������ ���������� ����������			*/
/************************************************/
#define 	SOL1DX  21120
#define		SOL1DY	-3200
#define		SOL2DX	21280
#define		SOL2DY	-20960
#define		SOL3DX	0
#define		SOL3DY	0	
#define		READDX	0
#define		READDY	-20800

extern uint32_t X,Y;
extern int Sol01_X,Sol01_Y; 		//���������� ���������� 0 ��������� 1
extern int Sol02_X,Sol02_Y; 		//���������� ���������� 0 ��������� 2
extern int Sol03_X,Sol03_Y; 		//���������� ���������� 0 ��������� 3
extern int Reader_X,Reader_Y; 		//���������� ���������� 0 ������

extern int sol01_dx;
extern int sol01_dy;
extern int sol02_dx;
extern int sol02_dy;
extern int sol03_dx;
extern int sol03_dy;


extern int8_t dir_X,dir_Y,dir_Z; 	//����������� ��������
extern uint16_t step_Delay;
extern uint16_t delays[255][2];
extern uint32_t step_Pulse_Width;
extern uint32_t dir_Change_Time;
extern uint8_t N;

typedef enum
{
  MOVMENT_ERR = 0,
  MOVMENT_OK = 1
}MOVMENT_STATE;
	
void step_X(uint16_t Delay_for_Step);
void step_Y(uint16_t Delay_for_Step);
void step_Z(uint16_t Delay_for_Step);
void step_XYZ(void);


void set_DIR_X_rear(void);
void set_DIR_X_front(void);
void set_DIR_Y_rear(void);
void set_DIR_Y_front(void);
void set_DIR_Z_rear(void);
void set_DIR_Z_front(void);


MOVMENT_STATE move_HEAD_X_TO( uint32_t nX);
MOVMENT_STATE move_HEAD_Y_TO( uint32_t nY);
MOVMENT_STATE move_HEAD_Z_TO( uint32_t nZ);
MOVMENT_STATE move_HEAD_TO( uint32_t mX, uint32_t mY);


MOVMENT_STATE move_HEAD_X( int32_t dX );
MOVMENT_STATE move_HEAD_Y( int32_t dY );
MOVMENT_STATE move_HEAD_Z( int32_t dZ );
MOVMENT_STATE move_HEAD( int32_t dX, int32_t dY );

MOVMENT_STATE go_HOME_X(void);
MOVMENT_STATE go_HOME_Y(void);
MOVMENT_STATE go_HOME_Z(void);
MOVMENT_STATE go_HOME(void);

MOVMENT_STATE search_PIPE(void);
void scan_PIPE(void);

MOVMENT_STATE go_TO_SOL01_0(void);
MOVMENT_STATE go_TO_SOL02_0(void);

void calc_Delays(void);
