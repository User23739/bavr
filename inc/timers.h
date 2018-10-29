/*
 * timers.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 14 // ���� ����� ��������

#define GTIMER1 0		//������ �������� ��������� ������ �
#define GTIMER2 1		//������ �������� ��������� ������ �
#define GTIMER3 2		//������ ������� ������������� ��������� ������ ��
#define GTIMER4 3		//������ ������� ������������� ��������� ������ ��
#define GTIMER5 4		//������ ������� ������������� ��������� ������ ��
#define GTIMER6 5		//������ ������� ������������� ��������� ������ B�
#define GTIMER7 6		//������ ������� ������������� ��������� ������ BB
#define GTIMER8 7		//������ ������� ������������� ��������� ������ BC
#define GTIMER9 8		//������ ������� ������������� ��������� ������ ��
#define GTIMER10 9		//������ ������� ������������� ��������� ������ ��
#define GTIMER11 10		//������ ������� ������������� ��������� ������ ��
#define GTIMER12 11		//������ ������� ������������� ��������� ������ ��
#define GTIMER13 12		//������ ������� ������������� ��������� ������ ��
#define GTIMER14 13		//������ ������� ������������� ��������� ������ ��
//������� ������ � ����������� ���������

void InitGTimers(void);			//������������� ���������� ��������
void StartGTimer(unsigned int GTimerID);		//����� �������
void StopGTimer(unsigned int GTimerID);			//������� �������
unsigned int GetGTimer(unsigned int GTimerID);	//��������� �������� �������� �������
void ProcessTimers(void);						//��������� ���������� �������

#endif /* TIMERS_H_ */
