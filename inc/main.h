
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
//#include <math.h">

/*----CMSIS и SPL----------*/
#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_adc.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_usart.h"
#include "stm32f2xx_tim.h"

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





#define KEY_DELAY 60

#define REF_MAX 10
#define REF_MIN -10
#define ZIRO 0

#define COUN_SINCH_ERR 20



//Сигнатуры функций
void Control(void);
void USARTSend(const unsigned char *pucBuffer);
void sin_compar_A(float *vol);
void sin_compar_B(float *vol);

void InitSynchA(void);
void SynchA (void);
void ZiroDetect(float *vol);

void SinQuadrant(int *x, int *y, float *buffA, float *buffB);

#endif /* MAIN_H_ */
