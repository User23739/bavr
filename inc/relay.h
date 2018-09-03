/*
 * relay.h
 *
 *  Created on: 6 ���. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef RELAY_H_
#define RELAY_H_



#define RELAY_1_PIN_IGBT        GPIO_Pin_6      ///< ��� ���������� ������������� ��� ���� 1
#define RELAY_1_PIN_ON          GPIO_Pin_3      ///< ��� ���������� ����������� ��� ���� 1
#define RELAY_1_PIN_OFF         GPIO_Pin_4      ///< ��� ���������� ����������� ��� ���� 1
#define RELAY_1_PORT_IGBT       GPIOD           ///< ���� ���������� ������������� ��� ���� 1
#define RELAY_1_PORT_ON         GPIOD           ///< ���� ���������� ����������� ��� ���� 1
#define RELAY_1_PORT_OFF        GPIOD           ///< ���� ���������� ����������� ��� ���� 1

#define RELAY_2_PIN_IGBT        GPIO_Pin_8      ///< ��� ���������� ������������� ��� ���� 2
#define RELAY_2_PIN_ON          GPIO_Pin_5      ///< ��� ���������� ����������� ��� ���� 2
#define RELAY_2_PIN_OFF         GPIO_Pin_6      ///< ��� ���������� ����������� ��� ���� 2
#define RELAY_2_PORT_IGBT       GPIOB           ///< ���� ���������� ������������� ��� ���� 2
#define RELAY_2_PORT_ON         GPIOB           ///< ���� ���������� ����������� ��� ���� 2
#define RELAY_2_PORT_OFF        GPIOB           ///< ���� ���������� ����������� ��� ���� 2

#define RELAY_3_PIN_IGBT        GPIO_Pin_3      ///< ��� ���������� ������������� ��� ���� 3
#define RELAY_3_PIN_ON          GPIO_Pin_1      ///< ��� ���������� ����������� ��� ���� 3
#define RELAY_3_PIN_OFF         GPIO_Pin_2      ///< ��� ���������� ����������� ��� ���� 3
#define RELAY_3_PORT_IGBT       GPIOE           ///< ���� ���������� ������������� ��� ���� 3
#define RELAY_3_PORT_ON         GPIOE           ///< ���� ���������� ����������� ��� ���� 3
#define RELAY_3_PORT_OFF        GPIOE           ///< ���� ���������� ����������� ��� ���� 3

#define RELAY_4_PIN_IGBT        GPIO_Pin_1      ///< ��� ���������� ������������� ��� ���� 4
#define RELAY_4_PIN_ON          GPIO_Pin_2      ///< ��� ���������� ����������� ��� ���� 4
#define RELAY_4_PIN_OFF         GPIO_Pin_0      ///< ��� ���������� ����������� ��� ���� 4
#define RELAY_4_PORT_IGBT       GPIOD           ///< ���� ���������� ������������� ��� ���� 4
#define RELAY_4_PORT_ON         GPIOD           ///< ���� ���������� ����������� ��� ���� 4
#define RELAY_4_PORT_OFF        GPIOD           ///< ���� ���������� ����������� ��� ���� 4

#define RELAY_5_PIN_IGBT        GPIO_Pin_7      ///< ��� ���������� ������������� ��� ���� 5
#define RELAY_5_PIN_ON          GPIO_Pin_4      ///< ��� ���������� ����������� ��� ���� 5
#define RELAY_5_PIN_OFF         GPIO_Pin_5      ///< ��� ���������� ����������� ��� ���� 5
#define RELAY_5_PORT_IGBT       GPIOD           ///< ���� ���������� ������������� ��� ���� 5
#define RELAY_5_PORT_ON         GPIOB           ///< ���� ���������� ����������� ��� ���� 5
#define RELAY_5_PORT_OFF        GPIOD           ///< ���� ���������� ����������� ��� ���� 5

#define RELAY_6_PIN_IGBT        GPIO_Pin_9      ///< ��� ���������� ������������� ��� ���� 6
#define RELAY_6_PIN_ON          GPIO_Pin_0      ///< ��� ���������� ����������� ��� ���� 6
#define RELAY_6_PIN_OFF         GPIO_Pin_7      ///< ��� ���������� ����������� ��� ���� 6
#define RELAY_6_PORT_IGBT       GPIOB           ///< ���� ���������� ������������� ��� ���� 6
#define RELAY_6_PORT_ON         GPIOE           ///< ���� ���������� ����������� ��� ���� 6
#define RELAY_6_PORT_OFF        GPIOB           ///< ���� ���������� ����������� ��� ���� 6

#define RELAY_BYPAS_PIN_1       GPIO_Pin_7      ///< ��� ���������� ����������� ��� ���� 6
#define RELAY_BYPAS_PIN_2       GPIO_Pin_8      ///< ��� ���������� ����������� ��� ���� 6
#define RELAY_BYPAS_PORT_1      GPIOC           ///< ���� ���������� ����������� ��� ���� 6
#define RELAY_BYPAS_PORT_2      GPIOC           ///< ���� ���������� ����������� ��� ���� 6

#define ADC_ZIRO                1987             ///< ������� ����

#define BUT1 					GPIO_Pin_9		///<������ ������������ �����������
#define BUT1_PORT				GPIOE			///<���� ���������� ������� ����������
void RELAY_Init( void );





#endif /* RELAY_H_ */
