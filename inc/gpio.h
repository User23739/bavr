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

void InitGPIO(void);
void ButControl(void);

#endif /* GPIO_H_ */
