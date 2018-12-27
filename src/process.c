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
short key = KEY_STAT_SW_RELAY;		    	//
extern short flag_aktiv_channel;
extern short flag_sinch_ch;
//extern short flag_gen_ban;
extern unsigned short reg_data[300];
extern short flag_freq[2];
extern short flag_sinch_ch;
extern short flag_z_switch_crash;
extern short flag_z_switch;
extern unsigned int key_delay;
extern unsigned int ss_key_delay;

//////////////////////////////////////////////////////////////////
// ���� �������� �������� ������� �����
#define MAX_BLOK 4
#define TRUE 1
#define FALSE 0
#define BLOK_1 0
#define BLOK_2 1
#define BLOK_3 2
#define BLOK_4 3


// ��������� ����������
uint8_t first_entry[MAX_BLOK];				//������� ���������
//static char prev_main_state;		//���������� ���������
//static char main_state_return;		//���������, � ������� ����� ��������� ����� ������-���� �������
//static char entry;					//���� ������� ��������� � ���������



///////////////////////////////////////////////////////////////////////




/*-----��������� ���������-----*/
#define REF_ZIRO 2048      //�������� 0 � ���������� ������
#define U_QUANTUM 0.35679742    //�������� 1 ������ � �������
#define AVER_N 2			//�������� ����������

#define ERR_C_CH 4  	//����������� ���������� ������ �� �����
#define TRU_C_CH 2000     //������������ ������������� �������� ��� ���������
#define BUFER_CIZE 320    //������ ������
#define DEPTH 79		 //������� ���������� ��� ���

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


//---------- ������� �������� ������� � ������������------------------------------------------------------------------------

void ChannelStatus(void){
	static int count_true[2];
	static int count_false[2];

	if ((flag_channel[0])&&(flag_channel[1])&&(flag_channel[2])&&(flag_freq[0])){
		count_true[0]++;
		count_false[0] = 0;
	}
	else{
		if(count_false[0] >= ERR_C_CH - 1) count_true[0] = 0;
		count_false[0]++;
	}
	if ((flag_channel[3])&&(flag_channel[4])&&(flag_channel[5])&&(flag_freq[1])){
		count_false[1] = 0;
		count_true[1]++;
	}
	else{
		if(count_false[1] >= ERR_C_CH - 1) count_true[1] = 0;
		count_false[1]++;
	}
	if (count_true[0]>= TRU_C_CH) status_chann_A = 1;
	if (count_false[0]>=ERR_C_CH) status_chann_A = 0;
	if (count_true[1]>= TRU_C_CH) status_chann_B = 1;
	if (count_false[1]>=ERR_C_CH) status_chann_B = 0;

	if (count_false[0] >= ERR_C_CH + 5) count_false[0] = ERR_C_CH + 1;
	if (count_false[1] >= ERR_C_CH + 5) count_false[1] = ERR_C_CH + 1;

	if (count_true[0] >= TRU_C_CH + 5 ) count_true[0] = TRU_C_CH +1;
	if (count_true[1] >= TRU_C_CH + 5) count_true[1] = TRU_C_CH +1;


}

//--------������� ������������ -------------------------------------------------------
void SwitchChannel(void){
	switch(key){
		case KEY_STAT_SW_RELAY:
			StatSwRelaySwitch();
			break;
		case KEY_STAT_SW:
			//StatSwSwitch();
			break;
		case KEY_RELAY:
			//RelaySwitch();
			break;
		/*case KEY_STAT_SW_RELAY_POL:
			StatSwRelayPolSwitch();
			break;*/
		default:
			break;
	}

}



void StatSwRelaySwitch(void){


	 if((status_chann_A)&&(status_chann_B)){

		switch (flag_priori_chann_manual){

				case CHANNAL_A:
					switch(flag_aktiv_channel){
						case ACTIV_OFF: //��������� � ����
							StartGTimer(GT_INIT_KEY);
							if(GetGTimer(GT_INIT_KEY) >key_delay){
								ChannelAOnSS(TIMER_ON);
								ChannelAOnRelay(TIMER_ON);
								StopGTimer(GT_INIT_KEY);
								}
							break;
						case ACTIV_CH_A: //������ �� ������

							break;
						case ACTIV_CH_B: //������� � ������ �
							switch(flag_sinch_ch){
									case SINKH_OFF:
										if (!first_entry[BLOK_1]){
											ChannelBOnSS(TIMER_OFF);
											ChannelBOffRelay(TIMER_ON);
											StartGTimer(GT_KEY_CH_B);
											first_entry[BLOK_1] = TRUE;
										}
										if(GetGTimer(GT_KEY_CH_B) >= key_delay){
											if(!first_entry[BLOK_2]){
												ChannelBOffSS();
												PauseGTimer(GT_KEY_CH_B);
												StartGTimer(GT_SS_CH_B);
												first_entry[BLOK_2] = TRUE;
											}
											if(GetGTimer(GT_SS_CH_B) >ss_key_delay){
												ChannelAOnSS(TIMER_ON);
												ChannelAOnRelay(TIMER_ON);
												StopGTimer(GT_KEY_CH_B);
												StopGTimer(GT_SS_CH_B);
												first_entry[BLOK_1] = FALSE;
												first_entry[BLOK_2] = FALSE;
											}

										}

										break;
									case SINKH_ON:
										ChannelBOffSS();
										ChannelBOffRelay(TIMER_ON);
										ChannelAOnSS(TIMER_ON);
										ChannelAOnRelay(TIMER_ON);

									break;
									default:
										break;
								}
							break;
						default:
							break;
						}
					break;
				case CHANNAL_B:
					switch(flag_aktiv_channel){
						case ACTIV_OFF:
							StartGTimer(GT_INIT_KEY);
							if(GetGTimer(GT_INIT_KEY) >key_delay){
								ChannelBOnSS(TIMER_ON);
								ChannelBOnRelay(TIMER_ON);
								StopGTimer(GT_INIT_KEY);

							}
							break;
						case ACTIV_CH_A:
							switch(flag_sinch_ch){
								case SINKH_OFF:
									if (!first_entry[BLOK_3]){
										ChannelAOnSS(TIMER_OFF);
										ChannelAOffRelay(TIMER_ON);
										StartGTimer(GT_KEY_CH_A);
										first_entry[BLOK_3] = TRUE;
									}
									if(GetGTimer(GT_KEY_CH_A) >= key_delay){
										if(!first_entry[BLOK_4]){
											ChannelAOffSS();
											StartGTimer(GT_SS_CH_A);
											first_entry[BLOK_4] = TRUE;
										}
										if(GetGTimer(GT_SS_CH_A) >ss_key_delay){
											ChannelBOnSS(TIMER_ON);
											ChannelBOnRelay(TIMER_ON);
											StopGTimer(GT_KEY_CH_A);
											StopGTimer(GT_SS_CH_A);
											first_entry[BLOK_3] = FALSE;
											first_entry[BLOK_4] = FALSE;
										}

									}

									break;
								case SINKH_ON:
									ChannelBOffSS();
									ChannelBOffRelay(TIMER_ON);
									ChannelAOnSS(TIMER_ON);
									ChannelAOnRelay(TIMER_ON);

								break;
								default:
									break;
								}
							break;
						case ACTIV_CH_B:

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
			case ACTIV_OFF:
			StartGTimer(GT_INIT_KEY);
			if(GetGTimer(GT_INIT_KEY) >key_delay){
				ChannelAOnSS(TIMER_ON);

				ChannelAOnRelay(TIMER_ON);

				StopGTimer(GT_INIT_KEY);
				}
				break;
			case ACTIV_CH_A:

				break;
			case ACTIV_CH_B:
				switch(flag_sinch_ch){
					case SINKH_OFF:
						if (!first_entry[BLOK_1]){
							ChannelBOffSS();
							ChannelBOffRelay(TIMER_ON);

							StartGTimer(GT_KEY_CH_B);
							first_entry[BLOK_1] = TRUE;
						}

						if(GetGTimer(GT_KEY_CH_B) >key_delay){


							if(!first_entry[BLOK_2]){
								ChannelBOffSS();
								StartGTimer(GT_SS_CH_B);
								first_entry[BLOK_2] = TRUE;
							}
							if(GetGTimer(GT_SS_CH_B) >ss_key_delay){
								ChannelAOnSS(TIMER_ON);

								ChannelAOnRelay(TIMER_ON);

								StopGTimer(GT_KEY_CH_B);
								StopGTimer(GT_SS_CH_B);
								first_entry[BLOK_1] = FALSE;
								first_entry[BLOK_2] = FALSE;
							}

						}

						break;
					case SINKH_ON:
						ChannelBOffSS();
						ChannelBOffRelay(TIMER_ON);

						ChannelAOnSS(TIMER_ON);

						ChannelAOnRelay(TIMER_ON);

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
			case ACTIV_OFF:
				StartGTimer(GT_INIT_KEY);
				if(GetGTimer(GT_INIT_KEY) >key_delay){
					ChannelBOnSS(TIMER_ON);

					ChannelBOnRelay(TIMER_ON);

					StopGTimer(GT_INIT_KEY);

				}
				break;
			case ACTIV_CH_A:
				switch(flag_sinch_ch){
					case SINKH_OFF:
						if (!first_entry[BLOK_3]){
							ChannelAOffSS();
							ChannelAOffRelay(TIMER_ON);

							StartGTimer(GT_KEY_CH_A);
							first_entry[BLOK_3] = TRUE;
						}

						if(GetGTimer(GT_KEY_CH_A) >key_delay){
							if(!first_entry[BLOK_4]){
								ChannelAOffSS();
								StartGTimer(GT_SS_CH_A);
								first_entry[BLOK_4] = TRUE;
							}
							if(GetGTimer(GT_SS_CH_A) >ss_key_delay){
								ChannelBOnSS(TIMER_ON);

								ChannelBOnRelay(TIMER_ON);

								StopGTimer(GT_KEY_CH_A);
								StopGTimer(GT_SS_CH_A);
								first_entry[BLOK_3] = FALSE;
								first_entry[BLOK_4] = FALSE;

							}

						}

						break;
					case SINKH_ON:
						ChannelAOffSS();
						ChannelAOffRelay(TIMER_ON);

						ChannelBOnSS(TIMER_ON);

						ChannelBOnRelay(TIMER_ON);

					break;
					default:
						break;
					}
				break;
			case ACTIV_CH_B:

				break;
			default:
				break;
		}

	}


	if((!status_chann_A)&&(!status_chann_B)){
		if(flag_aktiv_channel != 0){
			ChannelAOffSS();
			ChannelAOffRelay(TIMER_ON);

			ChannelBOffSS();
			ChannelBOffRelay(TIMER_ON);

			flag_aktiv_channel = 0;
		}

	}

}

/*void StatSwRelayPolSwitch(void){


}
*/
/*void StatSwSwitch(void){




}
void RelaySwitch(void){


}*/

/*������� ������� ������������������� ��������*/
void TrueRMS(){
//static float l_buffer[CHANN_W][BUFER_CIZE];
float period = 0;
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
		//rezult_true_rms[i] =roundl((sqrt(var[i]/DEPTH))-10);
		period = DEPTH*QUANT_POINT;
		rezult_true_rms[i] =roundl((sqrt((var[i]*QUANT_POINT)/period))-10);
}


}


