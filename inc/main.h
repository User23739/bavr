
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
#include <math.h>
#include "stdbool.h"
//#include <math.h">

/*----CMSIS � SPL----------*/
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
#include "spl.h"
#include "modbus.h"


/*----���������� ���������-------*/
#define CHANN_W 6  						//���������� ������� �������
#define KEY_DELAY 60 				 	//�������� �� ��������� ������
#define STOP_RELEY_TIMERS 10000			//������ �� ������������ ��������





//��������� �������
void Control(void);
void USARTSend(const unsigned char *pucBuffer);
void SinCompar(float *vol, float shift);
void ZeroDetect(float *vol);
void TrueRMS(float *vol);
void Freq();
void GenSin(void);
void TransData(void);
void MbWrite(void);




#endif /* MAIN_H_ */
