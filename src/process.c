/*
 * process.c
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */
#include "main.h"


extern uint16_t ADCBuffer[]; //переменная значений из АЦП
extern short int flag_channel[];
extern short flag_priori_chann_manual;	//переменная приоритека танала
short flag_status_chann[CHANN_W] = {0};				// состояние канала А;  0 - канал не в норме, 1 - канал в норм.
short status_chann_A = 0;		    		// 0 - откл; 1 - вкл
short status_chann_B = 0;		    		// 0 - откл; 1 - вкл
extern short flag_aktiv_channel;
extern short flag_sinch_ch;
//extern short flag_gen_ban;
extern unsigned short reg_data[300];
extern short flag_freq[2];
extern short flag_sinch_ch;
extern short flag_z_switch_crash;
extern short flag_z_switch;



/*-----Локальные константы-----*/
#define REF_ZIRO 2048      //значение 0 в абсолютных данных
#define U_QUANTUM 0.35679742    //значение 1 кванта в вольтах
#define AVER_N 2			//значение усреднения

#define ERR_C_CH 4  	//колличество допустимых ощибок на канал
#define TRU_C_CH 2000     //колличсество положительных отсчетов для включения
#define BUFER_CIZE 320    //размер буфера
#define DEPTH 79		 //глубина интеграции для РМС

//переменные для хранения текущих значений измерения
float rezult_true_rms[CHANN_W] = {0};

 float real_tmp_chan[CHANN_W] = {0}; // переменная куда помещаются измеренные данные
										// с АЦП
										// [0]-КАНАЛ А ФАЗА 1
										// [1]-КАНАЛ А ФАЗА 2
										// [2]-КАНАЛ А ФАЗА 3
										// [3]-КАНАЛ В ФАЗА 1
										// [4]-КАНАЛ В ФАЗА 2
										// [5]-КАНАЛ В ФАЗА 3
										// [6]-КАНАЛ С ФАЗА 1

 uint16_t  data_chan[CHANN_W] = {0}; // переменная куда помещаются измеренные данные
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
	static int aa[CHANN_W] = {0};			//указатели буфера
	static uint16_t raw_data_chan[CHANN_W][BUFER_CIZE];
	for (int i=0; i<CHANN_W; i++){

			if(aa[i] >= BUFER_CIZE - 1 ) aa[i] = 0;
			raw_data_chan[i][aa[i]] = vol[i];
			aa[i]++;

		}



}



 // переменные  буфера
// буфер кольцево для хранения данных измерения
float buff_chan[CHANN_W][BUFER_CIZE] = {0};                //двойной массив [x][y] x-колличество каналов, y- колличество ячеек
//указатель кольцевого буфера
int a[CHANN_W] = {0};

void BuffData(float *vol){

	for (int i=0; i<CHANN_W; i++){
		if(a[i] >= BUFER_CIZE - 1 ) a[i] = 0;
		buff_chan[i][a[i]] = vol[i];
		a[i]++;
	}
}


//---------- функция принятия решения о переключении------------------------------------------------------------------------

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

//--------функция переключения -------------------------------------------------------

void SwitchChannel(void){

	if((status_chann_A)&&(status_chann_B)){
		switch (flag_priori_chann_manual){
				case CHANNAL_A:
					switch(flag_aktiv_channel){
						case ACTIV_OFF:
							GPIO_SetBits(LED2_PORT, LED2);
							if(GetGTimer(GTIMER1) >KEY_DELAY){
								ChannelAON();
								GPIO_ResetBits(LED2_PORT, LED2);
								StopGTimer(GTIMER1);
								}
							break;
						case ACTIV_CH_A:
							//ничего не делаем
							break;
						case ACTIV_CH_B:
							//дополнительно проверяем на синхронность каналов
							switch(flag_sinch_ch){
									case SINKH_OFF:
										GPIO_SetBits(LED2_PORT, LED2);
										ChannelBOFF();
										if(flag_z_switch){
											ChannelAON();
											GPIO_ResetBits(LED2_PORT, LED2);

											}
										break;
									case SINKH_ON:
										GPIO_SetBits(LED2_PORT, LED2);
										ChannelBOFF();
										ChannelAON();
										GPIO_ResetBits(LED2_PORT, LED2);
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
							GPIO_SetBits(LED2_PORT, LED2);
							if(GetGTimer(GTIMER2) >KEY_DELAY){
								ChannelBON();
								StopGTimer(GTIMER2);
							GPIO_ResetBits(LED2_PORT, LED2);
							}
							break;
						case ACTIV_CH_A:
							//дополнительно проверяем на синхронность каналов
							switch(flag_sinch_ch){
									case SINKH_OFF:
										GPIO_SetBits(LED2_PORT, LED2);
										ChannelAOFF();
										if(flag_z_switch){
											ChannelBON();
											GPIO_ResetBits(LED2_PORT, LED2);
											}
										break;
									case SINKH_ON:
										GPIO_SetBits(LED2_PORT, LED2);
										ChannelAOFF();
										ChannelBON();
										GPIO_ResetBits(LED2_PORT, LED2);
										break;
									default:
										break;
								}
							break;
						case ACTIV_CH_B:
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
			case ACTIV_OFF:
				GPIO_SetBits(LED2_PORT, LED2);
				if(GetGTimer(GTIMER1) >KEY_DELAY){
					ChannelAON();
					GPIO_ResetBits(LED2_PORT, LED2);
					StopGTimer(GTIMER1);
					}
				break;
			case ACTIV_CH_A:
				//ничего не делаем
				break;
			case ACTIV_CH_B:
				//дополнительно проверяем на синхронность каналов
				switch(flag_sinch_ch){
						case SINKH_OFF:
							GPIO_SetBits(LED2_PORT, LED2);
							ChannelBOFF();
							if(flag_z_switch_crash){
								ChannelAON();
								GPIO_ResetBits(LED2_PORT, LED2);
								}
							break;
						case SINKH_ON:
							GPIO_SetBits(LED2_PORT, LED2);
							ChannelBOFF();
							ChannelAON();
							GPIO_ResetBits(LED2_PORT, LED2);
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
				GPIO_SetBits(LED2_PORT, LED2);
				if(GetGTimer(GTIMER2) >KEY_DELAY){
					ChannelBON();
					GPIO_ResetBits(LED2_PORT, LED2);
					StopGTimer(GTIMER2);
				}
				break;
			case ACTIV_CH_A:
				//дополнительно проверяем на синхронность каналов
				switch(flag_sinch_ch){
						case SINKH_OFF:
							GPIO_SetBits(LED2_PORT, LED2);
							ChannelAOFF();
							if(flag_z_switch_crash){
								ChannelBON();
								GPIO_ResetBits(LED2_PORT, LED2);
								}
							break;
						case SINKH_ON:
							GPIO_SetBits(LED2_PORT, LED2);
							ChannelAOFF();
							ChannelBON();
							GPIO_ResetBits(LED2_PORT, LED2);
							break;
						default:
							break;
					}
				break;
			case ACTIV_CH_B:
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

/*Функция расчета среднеквадратичного значения*/
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


