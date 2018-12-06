/*
 * timers.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define MAX_GTIMERS 27  // ���� ����� ��������

#define GTIMER1 0		//������ �������� ��������� ���� ������ �
#define GTIMER2 1		//������ �������� ��������� ���� ������ �
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
#define GTIMER15 14		//������ ������� ������������� ��������� ������ ��
#define GTIMER16 15		//������ ������� ������������� ��������� ������ ��
#define GT_ON_SS_A 16		//������ ������ ������ ���� ������ �
#define GT_ON_SS_B 17		//������ ������ ������ ���� ������ B
#define GTIMER19 18		//������ �������� ��������� ����������� ������ �
#define GTIMER20 19		//������ �������� ��������� ����������� ������ �
#define GT_INIT_KEY 20
#define GT_ON_RELAY_A 21
#define GT_OFF_RELAY_A 22
#define GT_ON_RELAY_B 23
#define GT_OFF_RELAY_B 24
#define GT_OFF_SS_A 25
#define GT_OFF_SS_B 26

//������� ������ � ����������� ���������

void InitGTimers(void);			//������������� ���������� ��������
void StartGTimer(unsigned int GTimerID);		//����� �������
void StopGTimer(unsigned int GTimerID);			//������� �������
unsigned int GetGTimer(unsigned int GTimerID);	//��������� �������� �������� �������
void ProcessTimers(void);						//��������� ���������� �������

#endif /* TIMERS_H_ */
