/*
 * timers.h
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 5  // макс число таймеров

#define GTIMER1 0		//таймер отключения канала А
#define GTIMER2 1		//таймер отключения канала В
#define GTIMER3 2		//таймер отключения каналов обоих
#define GTIMER4 3		//резерв
#define GTIMER5 4		//резерв

//Функции работы с глобальными таймерами

void InitGTimers(void);			//Инициализация глобальных таймеров
void StartGTimer(unsigned int GTimerID);		//Старт таймера
void StopGTimer(unsigned int GTimerID);			//Останов таймера
unsigned int GetGTimer(unsigned int GTimerID);	//Получение текущего значения таймера
void ProcessTimers(void);						//обработка прерываний таймера

#endif /* TIMERS_H_ */
