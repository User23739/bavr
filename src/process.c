/*
 * process.c
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint32_t ADCBuffer[]; //���������� �������� �� ���
extern short int flag_channel[];
extern short flag_priori_chann_manual;	//���������� ���������� ������
short flag_status_chann[7] = {0};				// ��������� ������ �;  0 - ����� �� � �����, 1 - ����� � ����.
short status_chann_A = 0;		    		// 0 - ����; 1 - ���
short status_chann_B = 0;		    		// 0 - ����; 1 - ���
extern short flag_aktiv_channel;
extern short flag_sinch_ch;
//extern short flag_gen_ban;

//���������� ��� �������� ������� �������� ���������
// float aver_tmp_chan[7] = {0}; // ���������� ���� ���������� ���������� ������
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
float buff_chanA1[1001] = {0};
float buff_chanA2[201] = {0};
float buff_chanA3[201] = {0};

float buff_chanB1[201] = {0};
float buff_chanB2[201] = {0};
float buff_chanB3[201] = {0};

float buff_chanC1[201] = {0};




//��������� ���������� ������
int a1 = 0;			//��������� ������ ������ �
int b1 = 0;			//��������� ������ ������ B
int c1 = 0;					//��������� ������ ������ �

void BuffData(float *vol){

	//static int a1, a2, a3;			//��������� ������ ������ �
	//static int b1, b2, b3;			//��������� ������ ������ B
	//static int c1;					//��������� ������ ������ �


	if(a1 >= 1000 ) a1 = 0;
	buff_chanA1[a1] = vol[0];
	buff_chanA2[a1] = vol[1];
	buff_chanA3[a1] = vol[2];
	a1++;
	if(b1  >= 200) b1 = 0;
	buff_chanB1[b1] = vol[3];
	buff_chanB2[b1] = vol[4];
	buff_chanB3[b1] = vol[5];
	b1++;
	if(c1  >= 200 ) c1 = 0;
	buff_chanC1[c1] = vol[6];
	c1++;
}

//������� ���������� � ��������� ������� ��������

//int count_mes = 0;
//short int flag_end_aver = 0;   //0-����� ������; 1- ������ ������
/*void Aver(void){
	int xa1 = a1;
	int da1 = 0;
	int xb1 = b1;
	int db1 = 0;
	int xc1 = c1;
	int dc1 = 0;

	if (xa1 == 0) da1 = 199;
	da1 = xa1-1;
	if (xb1 == 0) db1 = 199;
	db1 = xb1-1;
	if (xc1 == 0) dc1 = 199;
	dc1 = xc1-1;
	aver_tmp_chan[0] = roundl((buff_chanA1[xa1]+buff_chanA1[da1])/AVER_N);
	aver_tmp_chan[1] = roundl((buff_chanA2[xa1]+buff_chanA2[da1])/AVER_N);
	aver_tmp_chan[2] = roundl((buff_chanA3[xa1]+buff_chanA3[da1])/AVER_N);
	aver_tmp_chan[3] = roundl((buff_chanB1[xb1]+buff_chanB1[db1])/AVER_N);
	aver_tmp_chan[4] = roundl((buff_chanB2[xb1]+buff_chanB2[db1])/AVER_N);
	aver_tmp_chan[5] = roundl((buff_chanB3[xb1]+buff_chanB3[db1])/AVER_N);
	aver_tmp_chan[6] = roundl((buff_chanC1[xc1]+buff_chanC1[dc1])/AVER_N);


	/*for (int i=0; i<7; i++){
			aver_tmp_chan[i] = aver_tmp_chan[i]/2;
			aver_tmp_chan[i] = roundl(aver_tmp_chan[i]);
			}

	//for (int i=0; i<7; i++) aver_tmp_chan[i] = 0;*/



/*}*/


//----------������� ������������------------------------------------------------------------------------

void ChannelStatus(void){

	for (int i=0; i<7; i++){
		switch (flag_channel[i]){
			case 0:
				flag_status_chann[i] = 0;
				send_buffer_flag(i);
				send_buffer_flag(111);
				break;
			case 1:
				flag_status_chann[i] = 1;
				send_buffer_flag(i);
				send_buffer_flag(222);

				break;
			default:
				break;
		}
	}
	if ((flag_status_chann[0])&&(flag_status_chann[1])&&(flag_status_chann[2])){
		status_chann_A = 1;
	}
	else{
		status_chann_A = 0;
	}
	if ((flag_status_chann[3])&&(flag_status_chann[4])&&(flag_status_chann[5])){
		status_chann_B = 1;
	}
	else{
		status_chann_B = 0;
	}
}

//--------������� �������� ������� � ������������-------------------------------------------------------

void SwitchChannel(void){

	if((status_chann_A)&&(status_chann_B)){
		switch (flag_priori_chann_manual){
				case 0:
					switch(flag_aktiv_channel){
						case 0:
							if(GetGTimer(GTIMER1) >KEY_DELAY){
								ChannelAON();
								StopGTimer(GTIMER1);
								}
							break;
						case 1:
							//������ �� ������
							break;
						case 2:
							//������������� ��������� �� ������������ �������
							switch(flag_sinch_ch){
									case 0:
										ChannelBOFF();
										if(GetGTimer(GTIMER1) >KEY_DELAY){
											ChannelAON();
											StopGTimer(GTIMER1);
											}
										break;
									case 1:
										ChannelBOFF();
										ChannelAON();
										break;
									default:
										break;
								}
							break;
						default:
							break;
						}
					break;
				case 1:
					switch(flag_aktiv_channel){
						case 0:
							if(GetGTimer(GTIMER2) >KEY_DELAY){
								ChannelBON();
								StopGTimer(GTIMER2);
							}
							break;
						case 1:
							//������������� ��������� �� ������������ �������
							switch(flag_sinch_ch){
									case 0:
										ChannelAOFF();
										if(GetGTimer(GTIMER2) >KEY_DELAY){
											ChannelBON();
											StopGTimer(GTIMER2);
											}
										break;
									case 1:
										ChannelAOFF();
										ChannelBON();
										break;
									default:
										break;
								}
							break;
						case 2:
							//��� ������ �� ������
							break;
						default:
							break;
					}
					break;
				default:
					break;


			}
	}

	if((status_chann_A)&&(!status_chann_B)){
		switch(flag_aktiv_channel){
			case 0:
				if(GetGTimer(GTIMER1) >KEY_DELAY){
					ChannelAON();
					StopGTimer(GTIMER1);
					}
				break;
			case 1:
				//������ �� ������
				break;
			case 2:
				//������������� ��������� �� ������������ �������
				switch(flag_sinch_ch){
						case 0:
							ChannelBOFF();
							if(GetGTimer(GTIMER1) >KEY_DELAY){
								ChannelAON();
								StopGTimer(GTIMER1);
								}
							break;
						case 1:
							ChannelBOFF();
							ChannelAON();
							break;
						default:
							break;
					}
				break;
			default:
				break;
			}

	}
	if((!status_chann_A)&&(status_chann_B)){
		switch(flag_aktiv_channel){
			case 0:
				if(GetGTimer(GTIMER2) >KEY_DELAY){
					ChannelBON();
					StopGTimer(GTIMER2);
				}
				break;
			case 1:
				//������������� ��������� �� ������������ �������
				switch(flag_sinch_ch){
						case 0:
							ChannelAOFF();
							if(GetGTimer(GTIMER2) >KEY_DELAY){
								ChannelBON();
								StopGTimer(GTIMER2);
								}
							break;
						case 1:
							ChannelAOFF();
							ChannelBON();
							break;
						default:
							break;
					}
				break;
			case 2:
				//��� ������ �� ������
				break;
			default:
				break;
		}

	}
	if((!status_chann_A)&&(!status_chann_B)){
		ChannelAOFF();
		ChannelBOFF();
		flag_aktiv_channel = 0;

	}
}



