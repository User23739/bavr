/*
 * process.c
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint32_t ADCBuffer[]; //переменная значений из АЦП
extern short int flag_channel[];
//extern short int flag_channel_B[];
extern short flag_priori_chann_manual;	//переменная приоритека танала
short flag_status_chann_A = 0;				// состояние канала А;  0 - канал не в норме, 1 - канал в норм.
short flag_status_chann_B = 0;				// состояние канала B;  0 - канал не в норме, 1 - канал в норм.
short flag_switch_A = 0;		    		// 0 - откл; 1 - вкл
short flag_switch_B = 0;		    		// 0 - откл; 1 - вкл
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
//судить о качестве измерительного канала будем по 3 последовательным отказам по любой фазе на калане

//int count_work = 0;

void ChannelStatus(void){
	static int count_work;
	static int count_true_work[2];				//0-A; 1-B
	static int count_err_A[3] = {0};			// [1]- AA; [2]-AB; [3]-AC;
	static int count_err_B[3] = {0};			// [1]- BA; [2]-BB; [3]-BC;
	count_work++;
	send_buffer_flag(0);

	if ((flag_channel[0]==0)&&(flag_channel[1]==0)&&(flag_channel[2]==0)){
		if(count_true_work[0] >= TRUE_STEP) flag_status_chann_A = 1, send_buffer_flag(11);
		count_true_work[0]++;
		send_buffer_flag(12);

		//flag_gen_ban = 1;
	}
	else{
		send_buffer_flag(10);
		if (flag_channel[0]==1) count_err_A[1] += 1;
		if (flag_channel[1]==1) count_err_A[2] += 1;
		if (flag_channel[2]==1) count_err_A[3] += 1;
		if ((count_work == ERR_C_CH)&&((count_err_A[1] == ERR_C_CH)||(count_err_A[2] == ERR_C_CH)||(count_err_A[3] == ERR_C_CH))) flag_status_chann_A = 0, send_buffer_flag(13);

	}

	if ((flag_channel[4]==0)&&(flag_channel[5]==0)&&(flag_channel[6]==0)){
		if(count_true_work[1] >= TRUE_STEP) flag_status_chann_B = 1;
		count_true_work[1]++;
		//flag_gen_ban = 1;
	}
	else{
		if (flag_channel[4]==1) count_err_B[1] += 1;
		if (flag_channel[5]==1) count_err_B[2] += 1;
		if (flag_channel[6]==1) count_err_B[3] += 1;
		if ((count_work == ERR_C_CH)&&((count_err_B[1] == ERR_C_CH)||(count_err_B[2] == ERR_C_CH)||(count_err_B[3] == ERR_C_CH))) flag_status_chann_B = 0 ;

	}
	if (count_work == ERR_C_CH) {
		for (int i=0; i<3; i++)count_err_A[i] = 0;
		for (int i=0; i<3; i++)count_err_B[i] = 0;
		count_work = 0;
	}

}

//--------функция принятия решения о переключении-------------------------------------------------------

void SwitchChannel(void){

	if((flag_status_chann_A)&&(flag_status_chann_B)){
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

	if((flag_status_chann_A)&&(!flag_status_chann_B)){
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
	if((!flag_status_chann_A)&&(flag_status_chann_B)){
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
	if((!flag_status_chann_A)&&(!flag_status_chann_B)){
		ChannelAOFF();
		ChannelBOFF();
		flag_aktiv_channel = 0;

	}







}



