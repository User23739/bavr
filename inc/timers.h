/*
 * timers.h
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 14 // макс число таймеров

#define GTIMER1 0		//таймер отсрочки включения канала А
#define GTIMER2 1		//таймер отсрочки включения канала В
#define GTIMER3 2		//Таймер частоты положительной полуволны канала АА
#define GTIMER4 3		//Таймер частоты положительной полуволны канала АВ
#define GTIMER5 4		//Таймер частоты положительной полуволны канала АС
#define GTIMER6 5		//Таймер частоты положительной полуволны канала BА
#define GTIMER7 6		//Таймер частоты положительной полуволны канала BB
#define GTIMER8 7		//Таймер частоты положительной полуволны канала BC
#define GTIMER9 8		//Таймер частоты отрицательной полуволны канала АА
#define GTIMER10 9		//Таймер частоты отрицательной полуволны канала АВ
#define GTIMER11 10		//Таймер частоты отрицательной полуволны канала АС
#define GTIMER12 11		//Таймер частоты отрицательной полуволны канала ВА
#define GTIMER13 12		//Таймер частоты отрицательной полуволны канала ВВ
#define GTIMER14 13		//Таймер частоты отрицательной полуволны канала ВС
//Функции работы с глобальными таймерами

void InitGTimers(void);			//Инициализация глобальных таймеров
void StartGTimer(unsigned int GTimerID);		//Старт таймера
void StopGTimer(unsigned int GTimerID);			//Останов таймера
unsigned int GetGTimer(unsigned int GTimerID);	//Получение текущего значения таймера
void ProcessTimers(void);						//обработка прерываний таймера

#endif /* TIMERS_H_ */
