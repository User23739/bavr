/*
 * process.c
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint32_t ADCBuffer[]; 		//���������� �������� �� ���
extern short flag_priori_chann_manual;	//���������� ���������� ������


// ����������  ������
// ����� �������� ��� �������� ������ ���������
uint32_t buff_chanA1[201] = {0};
uint32_t buff_chanA2[201] = {0};
uint32_t buff_chanA3[201] = {0};

uint32_t buff_chanB1[201] = {0};
uint32_t buff_chanB2[201] = {0};
uint32_t buff_chanB3[201] = {0};

uint32_t buff_chanC1[201] = {0};

//��������� ���������� ������
int a1, a2, a3 = 0;			//��������� ������ ������ �
int b1, b2, b3 = 0;			//��������� ������ ������ B
int c1 = 0;					//��������� ������ ������ �


void send_buffer(uint32_t *vol_tmp_chan){

	if(a1 == 201 ) a1 = 0;
	buff_chanA1[a1] = vol_tmp_chan[0];
	a1++;
	if(a2 == 201 ) a2 = 0;
	buff_chanA2[a2] = vol_tmp_chan[1];
	a2++;
	if(a3 == 201 ) a3 = 0;
	buff_chanA3[a3] = vol_tmp_chan[2];
	a3++;
	if(b1 == 201 ) b1 = 0;
	buff_chanB1[b1] = vol_tmp_chan[3];
	b1++;
	if(b2 == 201 ) b2 = 0;
	buff_chanB2[b2] = vol_tmp_chan[4];
	b2++;
	if(b3 == 201 ) b3 = 0;
	buff_chanB3[b3] = vol_tmp_chan[5];
	b3++;
	if(c1 == 201 ) c1 = 0;
	buff_chanC1[c1] = vol_tmp_chan[6];
	c1++;
}

//������� ���������� � ��������� ������� ��������
int count_mes = 0;


void Aver(void){

	if (count_mes != 3 ){

	    for (int i=0; i<7; i++){
	    	aver_tmp_chan[i] += ADCBuffer[i];
			}
	    count_mes++;
	}
	else{
		for (int i=0; i<7; i++){
			aver_tmp_chan[i] = aver_tmp_chan[i]/3;
			}
		//��� �������� ������� ��������� ������ � �����
		send_buffer(aver_tmp_chan);
		count_mes = 0;
		sin_compar_A(aver_tmp_chan);	//�������� ������� ��������� ������ �
		sin_compar_B(aver_tmp_chan);	//�������� ������� ��������� ������ B

		for (int i=0; i<7; i++) aver_tmp_chan[i] = 0;
	}


}


/----------������� ������������------------------------------------------------------------------------
//������ � �������� �������������� ������ ����� ������ �� 3 ���������������� ������� �� ����� ����
int count_err_A[4] = {0};			//[0]- ����� �� �����; [1]- AA; [2]-AB; [3]-AC;
int count_err_B[4] = {0};			//[0]- ����� �� �����; [1]- BA; [2]-BB; [3]-BC;

void channel_status(void){

	count_err_A[0]++;
	if ((flag_channel_A[0]==0)&&(flag_channel_A[1]==0)&&(flag_channel_A[2]==0)){
		flag_status_chann_A = 0;
	}
	else{
		if (flag_channel_A[0]==1) count_err_A[1] += 1;
		if (flag_channel_A[1]==1) count_err_A[2] += 1;
		if (flag_channel_A[2]==1) count_err_A[3] += 1;
		if ((count_err_A[0] == 3)&&((count_err_A[1] == 3)||(count_err_A[2] == 3)||(count_err_A[3] == 3))) flag_status_chann_A = 1 ;
		if (count_err_A[0] == 3) for (int i=0; i<4; i++)count_err_A[i] = 0;
	}

	count_err_B[0]++;
	if ((flag_channel_B[0]==0)&&(flag_channel_B[1]==0)&&(flag_channel_B[2]==0)){
		flag_status_chann_B = 0;
	}
	else{
		if (flag_channel_B[0]==1) count_err_B[1] += 1;
		if (flag_channel_B[1]==1) count_err_B[2] += 1;
		if (flag_channel_B[2]==1) count_err_B[3] += 1;
		if ((count_err_B[0] == 3)&&((count_err_B[1] == 3)||(count_err_B[2] == 3)||(count_err_B[3] == 3))) flag_status_chann_B = 1 ;
		if (count_err_B[0] == 3) for (int i=0; i<4; i++)count_err_B[i] = 0;
	}


}

//--------������� �������� ������� � ������������-------------------------------------------------------

void switch_channel(void){
	if((flag_status_chann_A == 0)&&(flag_status_chann_B == 0)){
		if(flag_priori_chann_manual == 0){    //���� � ������ ����������� ����� �
			switch(flag_aktiv_channel){
				case 0:
					//���������� ���� �� ��������� ������ �
					flag_switch_A = 0;

					break;
				case 1:
					//������ �� ������
					break;
				case 2:
					//���������� ���� �� ��������� ������ �
					flag_switch_A = 0;

					break;
			}
		}
		else{  								//���� � ������ ����������� ����� �
			switch(flag_aktiv_channel){
				case 0:
					//���������� ���� �� ��������� ������ B
					flag_switch_B = 0;

					break;
				case 1:
					//���������� ���� �� ��������� ������ B
					flag_switch_B = 0;

					break;
				case 2:
					//������ �� ������
					break;
				}
		}
	}
	else if((flag_status_chann_A == 1)&&(flag_status_chann_B == 1)){
		flag_switch_A = 1;		//���� �� ���������� ������
		flag_switch_B = 1;		//���� �� ���������� ������
	}
	else{
		if (flag_status_chann_A == 1){
			flag_switch_A = 1;		//���� �� ���������� ������
			flag_switch_B = 0;		//���� �� ��������� ������


		}
		if (flag_status_chann_B == 1){
			flag_switch_B = 1;		//���� �� ���������� ������
			flag_switch_A = 0;		//���� �� ��������� ������

		}

	}

}



