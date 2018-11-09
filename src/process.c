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
extern unsigned short reg_data[300];
extern short flag_freq[2];
extern short flag_sinch_ch;



/*-----��������� ���������-----*/
#define REF_ZIRO 2048      //�������� 0 � ���������� ������
#define U_QUANTUM 0.35679742    //�������� 1 ������ � �������
#define AVER_N 2			//�������� ����������

#define ERR_C_CH 20  	//����������� ���������� ������ �� �����
#define TRU_C_CH 160     //������������ ������������� �������� ��� ���������
#define BUFER_CIZE 320    //������ ������
#define DEPTH 160		 //������� ���������� ��� ���

//���������� ��� �������� ������� �������� ���������
float rezult_true_rms[CHANN_W] = {0};

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





void TransRawDataToBuffer(uint16_t *vol){
	static int aa[CHANN_W] = {0};			//��������� ������
	static uint16_t raw_data_chan[CHANN_W][BUFER_CIZE];
	for (int i=0; i<CHANN_W; i++){

			if(aa[i] >= BUFER_CIZE - 1 ) aa[i] = 0;
			raw_data_chan[i][aa[i]] = vol[i];
			aa[i]++;

		}



}



 // ����������  ������
// ����� �������� ��� �������� ������ ���������
float buff_chan[CHANN_W][BUFER_CIZE] = {0};                //������� ������ [x][y] x-����������� �������, y- ����������� �����
//��������� ���������� ������
int a[CHANN_W] = {0};

void BuffData(float *vol){

	for (int i=0; i<CHANN_W; i++){
		if(a[i] >= BUFER_CIZE - 1 ) a[i] = 0;
		buff_chan[i][a[i]] = vol[i];
		a[i]++;
	}
}


//----------������� ������������------------------------------------------------------------------------

void ChannelStatus(void){
	static int count_true[2];
	static int count_false[2];
	//static int count_work;
	//count_work++;

	/*for (int i=0; i<CHANN_W; i++){
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
	}*/
	if ((flag_channel[0])&&(flag_channel[1])&&(flag_channel[2])/*&&(flag_freq[0])*/){

		//status_chann_A = 1;
		count_false[0] = 0;
		count_true[0]++;

		//send_buffer_flag(333);
	}
	else{
		//status_chann_A = 0;
		count_true[0] = 0;
		count_false[0]++;
		//send_buffer_flag(444);
	}
	if ((flag_channel[3])&&(flag_channel[4])&&(flag_channel[5])/*&&(flag_freq[1])*/){
		//status_chann_B = 1;
		count_false[1] = 0;
		count_true[1]++;
	}
	else{
		//status_chann_B = 0;
		count_true[1] = 0;
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
	if (count_true[0] >= TRU_C_CH + 5 ) count_true[0] = TRU_C_CH +1;
	if (count_true[1] >= TRU_C_CH + 5) count_true[1] = TRU_C_CH +1;
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

/*������� ������� ������������������� ��������*/
void TrueRMS(){
//static float l_buffer[CHANN_W][BUFER_CIZE];
//static float var_sum_n[CHANN_W];
static int l_p[CHANN_W];
static int bef_l_p[CHANN_W];
float var[CHANN_W] = {0};

	for (int i=0; i<CHANN_W; i++){
		bef_l_p[i] = l_p[i];
		l_p[i] =  a[i] ;
		var[i] = 0;
		for (int k=DEPTH; k>=0; k--){
			var[i] += buff_chan[i][l_p[i]] * buff_chan[i][l_p[i]];
			if(l_p[i] == 0 ) l_p[i] = BUFER_CIZE + 1;
			l_p[i]--;
		}
		rezult_true_rms[i] =roundl((sqrt(var[i]/DEPTH))-10);
}


}


