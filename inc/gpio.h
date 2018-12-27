/*
 * gpio.h
 *
 *  Created on: 29 ���. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef GPIO_H_
#define GPIO_H_

#define BUT1 					GPIO_Pin_9		///<������ ������������ �����������
#define BUT1_PORT				GPIOE			///<���� ���������� ������� ����������

#define LED1 					GPIO_Pin_15		///
#define LED1_PORT				GPIOE			///
#define LED2 					GPIO_Pin_13		///
#define LED2_PORT				GPIOE			///


void InitGPIO(void);
void ButControl(void);

#endif /* GPIO_H_ */
