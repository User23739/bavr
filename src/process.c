/*
 * process.c
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint16_t ADCBuffer[]; //���������� �������� �� ���
extern short int flag_channel[];
extern short flag_priori_chann_manual;	//���������� ���������� ������
short flag_status_chann[CHANN_W] = {0};				// ��������� ������ �;  0 - ����� �� � �����, 1 - ����� � ����.
short status_chann_A = 0;		    		// 0 - ����; 1 - ���
short status_chann_B = 0;		    		// 0 - ����; 1 - ���
extern short flag_aktiv_channel;
extern short flag_sinch_ch;
//extern short flag_gen_ban;



/*-----��������� ���������-----*/
#define REF_ZIRO 2048      //�������� 0 � ���������� ������
#define U_QUANTUM 0.35679742    //�������� 1 ������ � �������
#define AVER_N 2			//�������� ����������

#define ERR_C_CH 20  	//����������� ���������� ������ �� �����
#define TRU_C_CH 160     //������������ ������������� �������� ��� ���������


//���������� ��� �������� ������� �������� ���������

 float real_tmp_chan[CHANN_W] = {0}; // ���������� ���� ���������� ���������� ������
										// � ���
										// [0]-����� � ���� 1
										// [1]-����� � ���� 2
										// [2]-����� � ���� 3
										// [3]-����� � ���� 1
										// [4]-����� � ���� 2
										// [5]-����� � ���� 3
										// [6]-����� � ���� 1

 uint16_t  data_chan[CHANN_W] = {0}; // ���������� ���� ���������� ���������� ������
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
	for (int i=0; i<CHANN_W; i++){
		if(ADCBuffer[i] >4096 ){
			data_chan[i] = ADCBuffer[i] >> 4;
		}
		else{
			data_chan[i] = ADCBuffer[i];
		}

		real_tmp_chan[i] = roundl(((float)data_chan[i] - REF_ZIRO)*U_QUANTUM);
		}

}

uint16_t raw_data_chanA1[321] = {0};
uint16_t raw_data_chanA2[321] = {0};
uint16_t raw_data_chanA3[321] = {0};

uint16_t raw_data_chanB1[321] = {0};
uint16_t raw_data_chanB2[321] = {0};
uint16_t raw_data_chanB3[321] = {0};

int aa1 = 0;			//��������� ������ ������ �
int bb1 = 0;			//��������� ������ ������ B



void TransRawDataToBuffer(uint16_t *vol){

		if(aa1 >= 320 ) aa1 = 0;
		raw_data_chanA1[aa1] = vol[0];
		raw_data_chanA2[aa1] = vol[1];
		raw_data_chanA3[aa1] = vol[2];
		aa1++;
		if(bb1  >= 320) bb1 = 0;
		raw_data_chanB1[bb1] = vol[3];
		raw_data_chanB2[bb1] = vol[4];
		raw_data_chanB3[bb1] = vol[5];
		bb1++;
}



 // ����������  ������
// ����� �������� ��� �������� ������ ���������
float buff_chanA1[321] = {0};
float buff_chanA2[321] = {0};
float buff_chanA3[321] = {0};

float buff_chanB1[321] = {0};
float buff_chanB2[321] = {0};
float buff_chanB3[321] = {0};

float buff_chanC1[321] = {0};




//��������� ���������� ������
int a1 = 0;			//��������� ������ ������ �
int b1 = 0;			//��������� ������ ������ B
int c1 = 0;					//��������� ������ ������ �

void BuffData(float *vol){


	if(a1 >= 320 ) a1 = 0;
	buff_chanA1[a1] = vol[0];
	buff_chanA2[a1] = vol[1];
	buff_chanA3[a1] = vol[2];
	a1++;
	if(b1  >= 320) b1 = 0;
	buff_chanB1[b1] = vol[3];
	buff_chanB2[b1] = vol[4];
	buff_chanB3[b1] = vol[5];
	b1++;
	if(c1  >= 320 ) c1 = 0;
	buff_chanC1[c1] = vol[6];
	c1++;
}


//----------������� ������������------------------------------------------------------------------------

void ChannelStatus(void){
	static int count_true[2];
	static int count_false[2];
	//static int count_work;
	//count_work++;

	for (int i=0; i<CHANN_W; i++){
		switch (flag_channel[i]){
			case 0:
				flag_status_chann[i] = 0;
				//send_buffer_flag(i);
				//send_buffer_flag(111);
				break;
			case 1:
				flag_status_chann[i] = 1;
				//send_buffer_flag(i);
				//send_buffer_flag(222);

				break;
			default:
				break;
		}
	}
	if ((flag_status_chann[0])&&(flag_status_chann[1])&&(flag_status_chann[2])){

		//status_chann_A = 1;
		count_false[0] = 0;
		count_true[0]++;

		//send_buffer_flag(333);
	}
	else{
		//status_chann_A = 0;

		count_false[0]++;
		//send_buffer_flag(444);
	}
	if ((flag_status_chann[3])&&(flag_status_chann[4])&&(flag_status_chann[5])){
		//status_chann_B = 1;
		count_false[1] = 0;
		count_true[1]++;
	}
	else{
		//status_chann_B = 0;
		count_false[1]++;
	}


	if (count_true[0]>= TRU_C_CH) status_chann_A = 1;
	if (count_false[0]>=ERR_C_CH) status_chann_A = 0;
	if (count_true[1]>= TRU_C_CH) status_chann_B = 1;
	if (count_false[1]>=ERR_C_CH) status_chann_B = 0;

	//if(count_work >=2){
	if (count_false[0] >= ERR_C_CH + 5) count_false[0] = ERR_C_CH + 1;
	if (count_false[1] >= ERR_C_CH + 5) count_false[1] = ERR_C_CH + 1;
	//}
	//else if(count_work >= 160){
	if (count_true[0] >= TRU_C_CH + 5 ) count_true[0] = 0;
	if (count_true[1] >= TRU_C_CH + 5) count_true[1] = 0;
	//	count_work = 0;
	//}


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
	if(GetGTimer(GTIMER1) >STOP_RELEY_TIMERS){
		StopGTimer(GTIMER1);
	}
	if(GetGTimer(GTIMER2) >STOP_RELEY_TIMERS){
			StopGTimer(GTIMER2);
		}
}



