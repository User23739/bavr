/*
 * timers.c
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: a.boyarkin
 */
#include "main.h"

unsigned int GTimers[MAX_GTIMERS];


//��������� �������
#define TIMER_STOPPED 0  //������ ����������
#define TIMER_RUNNING 1  //������ ��������

char GTStates[MAX_GTIMERS]; // ������ �������� ������� �������� ��������

void InitGTimers(void){		//������������� ���������� ��������
	char i;
	for(i=0; i<MAX_GTIMERS; i++)
		GTStates[i] = TIMER_STOPPED;
}

void StartGTimer(unsigned int GTimerID){   //������ �������
	if (GTStates[GTimerID] == TIMER_STOPPED){
		GTimers[GTimerID] = 0;
		GTimers[GTimerID] = TIMER_RUNNING;
	}
}

void StopGTimer(unsigned int GTimerID){		//��������� �������
	GTimers[GTimerID] = TIMER_STOPPED;
}

unsigned int GetGTimer(unsigned int GTimerID){   //��������� �������� �������� �������
	return GTimers[GTimerID];
}

void ProcessTimers(void){		//���������� ����������
	char i;
	for(i=0; i<MAX_GTIMERS; i++)
		if(GTStates[i] == TIMER_RUNNING)
			GTimers[i]++;

}
