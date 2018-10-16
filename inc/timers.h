/*
 * timers.h
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 12 // макс число таймеров

#define GTIMER1 0		//таймер отсрочки включения канала А
#define GTIMER2 1		//таймер отсрочки включения канала В
#define GTIMER3 2		//резерв
#define GTIMER4 3		//таймер канала А
#define GTIMER5 4		//Таймер канала B
#define GTIMER6 5		//Таймер отсчетов канала АА
#define GTIMER7 6		//Таймер отсчетов канала АВ
#define GTIMER8 7		//Таймер отсчетов канала АС
#define GTIMER9 8		//Таймер отсчетов канала ВА
#define GTIMER10 9		//Таймер отсчетов канала ВВ
#define GTIMER11 10		//Таймер отсчетов канала ВС
#define GTIMER12 11		//Таймер отсчетов канала СА
//Функции работы с глобальными таймерами

void InitGTimers(void);			//Инициализация глобальных таймеров
void StartGTimer(unsigned int GTimerID);		//Старт таймера
void StopGTimer(unsigned int GTimerID);			//Останов таймера
unsigned int GetGTimer(unsigned int GTimerID);	//Получение текущего значения таймера
void ProcessTimers(void);						//обработка прерываний таймера

#endif /* TIMERS_H_ */
