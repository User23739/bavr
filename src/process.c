/*
 * process.c
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint32_t ADCBuffer[]; //���������� �������� �� ���
extern short int flag_channel_A[];
extern short int flag_channel_B[];
extern short flag_priori_chann_manual;	//���������� ���������� ������
extern short flag_status_chann_A;
extern short flag_status_chann_B;
extern short flag_switch_A;		    		// 0 - ���; 1 - ����
extern short flag_switch_B;
extern short flag_aktiv_channel;
//���������� ��� �������� ������� �������� ���������
 float aver_tmp_chan[7] = {0}; // ���������� ���� ���������� ���������� ������
										// � ���
										// [0]-����� � ���� 1
										// [1]-����� � ���� 2
										// [2]-����� � ���� 3
										// [3]-����� � ���� 1
										// [4]-����� � ���� 2
										// [5]-����� � ���� 3
										// [6]-����� � ���� 1
 float real_tmp_chan[7] = {0}; // ���������� ���� ���������� ���������� ������
										// � ���
										// [0]-����� � ���� 1
										// [1]-����� � ���� 2
										// [2]-����� � ���� 3
										// [3]-����� � ���� 1
										// [4]-����� � ���� 2
										// [5]-����� � ���� 3
										// [6]-����� � ���� 1

//������������ ������� ������

void TransInData(void){
	for (int i=0; i<7; i++){
		real_tmp_chan[i] = roundl(((float)ADCBuffer[i] - REF_ZIRO)*U_QUANTUM);
		}

}



 // ����������  ������
// ����� �������� ��� �������� ������ ���������
float buff_chanA1[201] = {0};
float buff_chanA2[201] = {0};
float buff_chanA3[201] = {0};

float buff_chanB1[201] = {0};
float buff_chanB2[201] = {0};
float buff_chanB3[201] = {0};

float buff_chanC1[201] = {0};




//��������� ���������� ������
int a1, a2, a3 = 0;			//��������� ������ ������ �
int b1, b2, b3 = 0;			//��������� ������ ������ B
int c1 = 0;					//��������� ������ ������ �

void BuffData(float *vol){

	//static int a1, a2, a3;			//��������� ������ ������ �
	//static int b1, b2, b3;			//��������� ������ ������ B
	//static int c1;					//��������� ������ ������ �


	if(a1 >= 200 ) a1 = 0;
	buff_chanA1[a1] = vol[0];
	a1++;
	if(a2  >= 200 ) a2 = 0;
	buff_chanA2[a2] = vol[1];
	a2++;
	if(a3  >= 200) a3 = 0;
	buff_chanA3[a3] = vol[2];
	a3++;
	if(b1  >= 200) b1 = 0;
	buff_chanB1[b1] = vol[3];
	b1++;
	if(b2  >= 200) b2 = 0;
	buff_chanB2[b2] = vol[4];
	b2++;
	if(b3  >= 200) b3 = 0;
	buff_chanB3[b3] = vol[5];
	b3++;
	if(c1  >= 200 ) c1 = 0;
	buff_chanC1[c1] = vol[6];
	c1++;
}








//������� ���������� � ��������� ������� ��������

//int count_mes = 0;
//short int flag_end_aver = 0;   //0-����� ������; 1- ������ ������
void Aver(void){
	int xa1 = a1;
	int xa2 = a2;
	int xa3 = a3;
	int xb1 = b1;
	int xb2 = b2;
	int xb3 = b3;
	int xc1 = c1;

	aver_tmp_chan[0] = roundl((buff_chanA1[xa1]+buff_chanA1[xa1-1])/AVER_N);
	aver_tmp_chan[1] = roundl((buff_chanA1[xa2]+buff_chanA1[xa2-1])/AVER_N);
	aver_tmp_chan[2] = roundl((buff_chanA1[xa3]+buff_chanA1[xa3-1])/AVER_N);
	aver_tmp_chan[3] = roundl((buff_chanA1[xb1]+buff_chanA1[xb1-1])/AVER_N);
	aver_tmp_chan[4] = roundl((buff_chanA1[xb2]+buff_chanA1[xb2-1])/AVER_N);
	aver_tmp_chan[5] = roundl((buff_chanA1[xb3]+buff_chanA1[xb3-1])/AVER_N);
	aver_tmp_chan[6] = roundl((buff_chanA1[xc1]+buff_chanA1[xc1-1])/AVER_N);


	/*for (int i=0; i<7; i++){
			aver_tmp_chan[i] = aver_tmp_chan[i]/2;
			aver_tmp_chan[i] = roundl(aver_tmp_chan[i]);
			}

	//for (int i=0; i<7; i++) aver_tmp_chan[i] = 0;*/



}


//----------������� ������������------------------------------------------------------------------------
//������ � �������� �������������� ������ ����� �� 3 ���������������� ������� �� ����� ���� �� ������
int count_err_A[4] = {0};			//[0]- ����� �� �����; [1]- AA; [2]-AB; [3]-AC;
int count_err_B[4] = {0};			//[0]- ����� �� �����; [1]- BA; [2]-BB; [3]-BC;
int count_work = 0;

void channel_status(void){
	if (flag_end_aver == 0){
		count_work++;
		if ((flag_channel_A[0]==0)&&(flag_channel_A[1]==0)&&(flag_channel_A[2]==0)){
			flag_status_chann_A = 0;
		}
		else{
			if (flag_channel_A[0]==1) count_err_A[1] += 1;
			if (flag_channel_A[1]==1) count_err_A[2] += 1;
			if (flag_channel_A[2]==1) count_err_A[3] += 1;
			if ((count_work == ERR_C_CH)&&((count_err_A[1] == ERR_C_CH)||(count_err_A[2] == ERR_C_CH)||(count_err_A[3] == ERR_C_CH))) flag_status_chann_A = 1 ;

		}

		if ((flag_channel_B[0]==0)&&(flag_channel_B[1]==0)&&(flag_channel_B[2]==0)){
			flag_status_chann_B = 0;
		}
		else{
			if (flag_channel_B[0]==1) count_err_B[1] += 1;
			if (flag_channel_B[1]==1) count_err_B[2] += 1;
			if (flag_channel_B[2]==1) count_err_B[3] += 1;
			if ((count_work == ERR_C_CH)&&((count_err_B[1] == ERR_C_CH)||(count_err_B[2] == ERR_C_CH)||(count_err_B[3] == ERR_C_CH))) flag_status_chann_B = 1 ;

		}
		if (count_work == ERR_C_CH) {
			for (int i=0; i<4; i++)count_err_A[i] = 0;
			for (int i=0; i<4; i++)count_err_B[i] = 0;
			count_work = 0;
		}
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



