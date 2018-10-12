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
// float aver_tmp_chan[7] = {0}; // переменная куда помещаются измеренные данные
										// с АЦП
										// [0]-КАНАЛ А ФАЗА 1
										// [1]-КАНАЛ А ФАЗА 2
										// [2]-КАНАЛ А ФАЗА 3
										// [3]-КАНАЛ В ФАЗА 1
										// [4]-КАНАЛ В ФАЗА 2
										// [5]-КАНАЛ В ФАЗА 3
										// [6]-КАНАЛ С ФАЗА 1
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
float buff_chanA1[1001] = {0};
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

	//static int a1, a2, a3;			//указатели буфера канала А
	//static int b1, b2, b3;			//указатели буфера канала B
	//static int c1;					//указатели буфера канала С


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

//функция усреднения и фильтация фходных значений

//int count_mes = 0;
//short int flag_end_aver = 0;   //0-можно мерить; 1- нельзя мерить
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


//----------функция переключения------------------------------------------------------------------------

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
}



