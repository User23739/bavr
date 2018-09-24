
/*
 * main.h
 *
 *  Created on: 6 ����. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef MAIN_H_
#define MAIN_H_

/*-----����������� ����������-----------*/
#include <stdint.h>
#include <stdio.h>
#include <time.h>
//#include <math.h">

/*----CMSIS � SPL----------*/
#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_adc.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_usart.h"
#include "stm32f2xx_tim.h"

/*----������----------*/
#include "system.h"
#include "relay.h"
#include "adc_dma_mesur.h"
#include "rs232.h"
#include "tim_mesure.h"
#include "timers.h"
#include "relay.h"
#include "process.h"
#include "gpio.h"





#define KEY_DELAY 45

#define REF_MAX 2100
#define REF_MIN 1800

#define COUN_SINCH_ERR 20



//��������� �������
void USARTSend(const unsigned char *pucBuffer);
void sin_compar_A(uint32_t *vol);
void sin_compar_B(uint32_t *vol);

void InitSynchA(void);
void SynchA (uint32_t *vol);


#endif /* MAIN_H_ */
