/*
 * process.c
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint32_t ADCBuffer[]; //переменная значений из АЦП
extern short int flag_channel[];
extern short flag_priori_chann_manual;	//переменная приоритека танала
short flag_status_chann[7] = {0};				// состояние канала А;  0 - канал не в норме, 1 - канал в норм.
short status_chann_A = 0;		    		// 0 - откл; 1 - вкл
short status_chann_B = 0;		    		// 0 - откл; 1 - вкл
extern short flag_aktiv_channel;
extern short flag_sinch_ch;
//extern short flag_gen_ban;

//переменные для хранения текущих значений измерения

 float real_tmp_chan[7] = {0}; // переменная куда помещаются измеренные данные
										// с АЦП
										// [0]-КАНАЛ А ФАЗА 1
										// [1]-КАНАЛ А ФАЗА 2
										// [2]-КАНАЛ А ФАЗА 3
										// [3]-КАНАЛ В ФАЗА 1
										// [4]-КАНАЛ В ФАЗА 2
										// [5]-КАНАЛ В ФАЗА 3
										// [6]-КАНАЛ С ФАЗА 1

//Нормализация входных данных

void TransInData(void){
	for (int i=0; i<7; i++){
		real_tmp_chan[i] = roundl(((float)ADCBuffer[i] - REF_ZIRO)*U_QUANTUM);
		}

}



 // переменные  буфера
// буфер кольцево для хранения данных измерения
float buff_chanA1[201] = {0};
float buff_chanA2[201] = {0};
float buff_chanA3[201] = {0};

float buff_chanB1[201] = {0};
float buff_chanB2[201] = {0};
float buff_chanB3[201] = {0};

float buff_chanC1[201] = {0};




//указатель кольцевого буфера
int a1 = 0;			//указатели буфера канала А
int b1 = 0;			//указатели буфера канала B
int c1 = 0;					//указатели буфера канала С

void BuffData(float *vol){

	if(a1 >= 200 ) a1 = 0;
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


//----------функция переключения------------------------------------------------------------------------

void ChannelStatus(void){
	static int count_work;
	static int count_true[2];
	static int count_false[2];
	count_work++;

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
		//count_true[0]++;
		//send_buffer_flag(333);
	}
	else{
		status_chann_A = 0;
		//count_false[0]++;
		//send_buffer_flag(444);
	}
	if ((flag_status_chann[3])&&(flag_status_chann[4])&&(flag_status_chann[5])){
		status_chann_B = 1;
		//count_true[1]++;
	}
	else{
		status_chann_B = 0;
		//count_false[1]++;
	}

	/*if ((count_work >= ERR_C_CH) && (count_true[0]>=ERR_C_CH)){
		status_chann_A = 1;
	}
	else if ((count_work >= ERR_C_CH) && (count_false[0]>=ERR_C_CH)){
		status_chann_A = 0;
	}
	if ((count_work >= ERR_C_CH) && (count_true[1]>=ERR_C_CH)){
		status_chann_B = 1;
	}
	else if ((count_work >= ERR_C_CH) && (count_false[1]>=ERR_C_CH)){
		status_chann_B = 0;
	}
	if (count_work >= ERR_C_CH+1){
		count_work = 0;
		count_true[0] = 0;
		count_true[1] = 0;
		count_false[0] = 0;
		count_false[1] = 0;

	}*/
}

//--------функция принятия решения о переключении-------------------------------------------------------

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
							//ничего не делаем
							break;
						case 2:
							//дополнительно проверяем на синхронность каналов
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
							//дополнительно проверяем на синхронность каналов
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
							//тут ничего не делаем
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
			(GTIMER1);
					}
				break;
			case 1:
				//ничего не делаем
				break;
			case 2:
				//дополнительно проверяем на синхронность каналов
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
				//дополнительно проверяем на синхронность каналов
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
				//тут ничего не делаем
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



