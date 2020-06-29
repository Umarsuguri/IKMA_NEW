#include "stm32f4xx.h"                  // Device header


/* Нулевой байт данных всегда 0xFF, который является признаком начала обмена */
#define START_BYTE 0xFF
/* Длина команды */
#define CMD_LENGHT 5
/* Первый байт обозначает типа данных */
#define CMD_STD 0x01 // Признак стандартных команд

/* Ниже перечислены стандартные команды */
#define START_MARKING 0x01//Начать маркировку
/*Параметры для команды "Начать маркировку"*/

#define GO_HOME 0x02//Вернуть каретку в исходную позицию
/*Параметры для команды "Вернуть каретку в исходную позицию"*/

#define WHASH 0x03//Промывка каплеструйной головки
/*Параметры для команды "Промывка каплеструйной головки"*/
#define WHASH_START	 0x01// Начать промывку
#define WHASH_STOP	 0x02// Остановить промывку

#define DATA_RX			 0x10//Команда на подготовку приема данных
/*Параметром для команды "Команда на подготовку приема данных" будет количество данных*/

#define DATA_TX			 0x11//Команда на передачу данных
/*Параметром для команды "Команда на передачу данных" тип передаваемых данных*/
#define SENS_DATA			 0x01//Передать данные со всех сенсоров
#define X_POS			 0x02//Передать координату X
#define Y_POS			 0x03//Передать координату Y
#define Z_POS			 0x04//Передать координату Z


#define CMD_CFG 0x02 // Признак конфигурационных команд

#define SET_SOL01_PW 0x31 //Установка силы удара
/*Параметром для команды "Установка силы удара" будет цифра от 1 - 9, от слабого удара к сильномуы*/
#define SET_SOL02_PW 0x32 //Установка длительности открытия клапана
/*Параметром для команды "Установка длительности открытия клапана" будет цифра от 1 - 9, 1 соответствует длительност открытия клапана в 200 мкс, каждый следующий параметр добавляет 40 мкс*/

#define SET_CALL_PAR_X  0x11 //установка DX, расстояние между головками по оси X
/*Переметром этой команды будет коэффицент помножаемый на 0.1мм   коэффицент от -127..128*/
#define SET_CALL_PAR_Y  0x12 //установка DX, расстояние между головками по оси Y
/*Переметром этой команды будет коэффицент помножаемый на 0.1мм */

#define CMD_TST 0x03// Признак команд тестового режима

#define SEARCH_PIPE 0x50//Поиск трубы (без параметров)

#define SCAN_PIPE_LT 0x51//Поиск середины трубы многократный
/*Параметры для команды "Поиск середины трубы многократный"*/
#define SCAN_PIPE_START 0x01// Начать сканирование
#define SCAN_PIPE_STOP 	0x02// Остановить сканирование

#define SCAN_PIPE_OT 0x52//Поиск середины трубы однократный (без параметров)

#define MARK_POINT  0x60 //Поставить точку

#define STRIKE_SOL01 0x61//Ударить иглоударкой
/*Параметром для команды "Ударить иглоударкой" будет цифра от 1 - 9, от слабого удара к сильномуы*/

#define STRIKE_SOL02 0x62//Плюнуть каплеструйкой
/*Параметром для команды "Плюнуть каплеструйкой" будет цифра от 1 - 9, 1 соответствует длительност открытия клапана в 200 мкс, каждый следующий параметр добавляет 40 мкс*/

/*Переменные для обработки комманд*/

extern uint16_t move_X[];
extern uint16_t move_Y[];
extern uint16_t k;
extern uint8_t cmd_rc,cmd_er;
extern UART_HandleTypeDef huart4;
extern uint8_t cmd[];
extern uint8_t cmd_RX[];
extern uint8_t cnt_RX;
extern uint8_t cnt_Mrk;
extern uint8_t cs;
void cmd_do(void);