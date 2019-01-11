
/*
 * main.h
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef MAIN_H_
#define MAIN_H_

/*-----Стандартные библиотеки-----------*/
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "stdbool.h"
//#include <сmath.h">

/*----CMSIS и SPL----------*/
/*#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_adc.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_usart.h"
#include "stm32f2xx_tim.h"
*/
/*----Логика----------*/
#include "system.h"
#include "relay.h"
#include "adc_dma_mesur.h"
#include "rs232.h"
#include "tim_mesure.h"
#include "timers.h"
#include "relay.h"
#include "process.h"
#include "gpio.h"
#include "spl.h"
#include "modbus.h"


/*----Глобальные константы-------*/
#define CHANN_W 7  						//количество рабочих каналов
//#define KEY_DELAY KEY_PAUSE 				 	//задержка на включение ключей
//#define KEY_STAT_SW_DELAY SS_PAUSE
#define STOP_RELEY_TIMERS 10000			//защита от переполнения таймеров
#define BUFER_CIZE 320 					//размер накопительног буфера
#define QUANT_POINT 0.00025				//единица времени квантования
#define CHANNAL_A 0
#define CHANNAL_B 1
#define ACTIV_OFF 0
#define ACTIV_CH_A 1
#define ACTIV_CH_B 2
#define SINKH_OFF 0
#define SINKH_ON 1
#define KEY_STAT_SW_RELAY 0 					//тип ключа статик свитч+реле
#define KEY_STAT_SW 1 						//тип ключа статик свитч
#define KEY_RELAY 2 						//тип ключа реле
#define KEY_STAT_SW_RELAY_POL 3 					//тип ключа статик свитч+реле
#define STAT_SW_DELAY 400
#define STAT_RELAY_DELAY 200
#define TIMER_ON 1
#define TIMER_OFF 0




//Сигнатуры функций
void Control(void);
void USARTSend(const unsigned char *pucBuffer);
void SinCompar(float *vol, float shift);
void ZeroDetect(float *vol);
void Freq();
void GenSin(void);
void TransData(void);
void MbWrite(void);
//bool approximatelyEqual(float a, float b, float epsilon);
void SinChanAB();
void Noise(float *vol);
void FastSwitch(float *vol);
void TestKeySwitch();




#endif /* MAIN_H_ */
