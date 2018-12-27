/*
 * timers.c
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: a.boyarkin
 */
#include "main.h"
//******************************************************************************
// Секция определения переменных, используемых в модуле
//******************************************************************************
//Состояния таймера
#define TIMER_STOPPED 0  //Таймер остановлен
#define TIMER_RUNNING 1  //Таймер работает
#define TIMER_PAUSED 2   //Таймер на паузе

//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
static uint16_t GTimers[MAX_GTIMERS];//массив значений глоб.таймеров
static char GTStates[MAX_GTIMERS];//массив текущих состояний глоб.таймеров

//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************

//******************************************************************************
// Секция описания функций
//******************************************************************************

void InitGTimers(void)
{
	unsigned char i;
	for (i = 0; i < MAX_GTIMERS; i++) {
		GTStates[i]=TIMER_STOPPED;
		//Ruslashka добавил GTimers[i] = 0; 12.04.2017
		GTimers[i] = 0;
	}
}

void StartGTimer(uint16_t GTimerID)
{
	if(GTStates[GTimerID]==TIMER_STOPPED)
	{
		GTimers[GTimerID] = 0;
		GTStates[GTimerID] = TIMER_RUNNING;
	}
}

void StopGTimer(uint16_t GTimerID)
{
	GTStates[GTimerID] = TIMER_STOPPED;
}

void ResetGTimer(uint16_t GTimerID)
{
	//if(GTStates[GTimerID]==TIMER_RUNNING)
	//{
		GTimers[GTimerID] = 0;
	//}
}

void PauseGTimer(uint16_t GTimerID)
{
	if(GTStates[GTimerID]==TIMER_RUNNING)
	{
		GTStates[GTimerID]=TIMER_PAUSED;
	}
}

void ReleaseGTimer(uint16_t GTimerID)
{
	if(GTStates[GTimerID]==TIMER_PAUSED)
	{
		GTStates[GTimerID]=TIMER_RUNNING;
	}
}

uint16_t GetGTimer(uint16_t GTimerID)
{
	return GTimers[GTimerID];
}

void ProcessTimers(void)
{
	unsigned char i;

	for (i = 0; i < MAX_GTIMERS; ++i) {
		if(GTStates[i]==TIMER_RUNNING)
		{
			if(GTimers[i] < 4294967294){
				GTimers[i]++;
			}
			else{
				GTimers[i] = 0;
			}
		}
	}
}
