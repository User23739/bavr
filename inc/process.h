/*
 * process.h
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define REF_ZIRO 2048      //�������� 0 � ���������� ������
#define U_QUANTUM 0.25513    //�������� 1 ������ � �������
#define AVER_N 2			//�������� ����������

#define ERR_C_CH 20  	//����������� ���������� ������ �� �����
#define TRU_C_CH 20     //������������ ������������� �������� ��� ���������



void TransInData(void);
void BuffData(float *vol);
void ChannelStatus(void);
void SwitchChannel(void);
//void Aver(void);

#endif /* PROCESS_H_ */
