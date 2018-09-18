/*
 * timers.c
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: a.boyarkin
 */
#include "main.h"

unsigned int GTimers[MAX_GTIMERS];


//Состояния таймера
#define TIMER_STOPPED 0  //Таймер остановлен
#define TIMER_RUNNING 1  //Таймер работает

char GTStates[MAX_GTIMERS]; // Массив хранящий текущие значения таймеров

void InitGTimers(void){		//Инициализация глобальных таймеров
	char i;
	for(i=0; i<MAX_GTIMERS; i++)
		GTStates[i] = TIMER_STOPPED;
}

void StartGTimer(unsigned int GTimerID){   //Запуск таймера
	if (GTStates[GTimerID] == TIMER_STOPPED){
		GTimers[GTimerID] = 0;
		GTimers[GTimerID] = TIMER_RUNNING;
	}
}

void StopGTimer(unsigned int GTimerID){		//Остановка таймера
	GTimers[GTimerID] = TIMER_STOPPED;
}

unsigned int GetGTimer(unsigned int GTimerID){   //Получение текущего значения таймера
	return GTimers[GTimerID];
}

void ProcessTimers(void){		//обработчик прерываний
	char i;
	for(i=0; i<MAX_GTIMERS; i++)
		if(GTStates[i] == TIMER_RUNNING)
			GTimers[i]++;

}
