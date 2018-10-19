/*
 * process.h
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define REF_ZIRO 2048      //значение 0 в абсолютных данных
#define U_QUANTUM 0.25513    //значение 1 кванта в вольтах
#define AVER_N 2			//значение усреднения

#define ERR_C_CH 20  	//колличество допустимых ощибок на канал
#define TRU_C_CH 20     //колличсество положительных отсчетов для включения



void TransInData(void);
void BuffData(float *vol);
void ChannelStatus(void);
void SwitchChannel(void);
//void Aver(void);

#endif /* PROCESS_H_ */
