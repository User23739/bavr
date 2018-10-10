/*
 * timers.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 5  // ���� ����� ��������

#define GTIMER1 0		//������ �������� ��������� ������ �
#define GTIMER2 1		//������ �������� ��������� ������ �
#define GTIMER3 2		//������
#define GTIMER4 3		//������ ������ �
#define GTIMER5 4		//������ ������ B

//������� ������ � ����������� ���������

void InitGTimers(void);			//������������� ���������� ��������
void StartGTimer(unsigned int GTimerID);		//����� �������
void StopGTimer(unsigned int GTimerID);			//������� �������
unsigned int GetGTimer(unsigned int GTimerID);	//��������� �������� �������� �������
void ProcessTimers(void);						//��������� ���������� �������

#endif /* TIMERS_H_ */
