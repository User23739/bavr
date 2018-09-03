/*
 * relay.h
 *
 *  Created on: 6 авг. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef RELAY_H_
#define RELAY_H_



#define RELAY_1_PIN_IGBT        GPIO_Pin_6      ///< Пин управления транзисторами для реле 1
#define RELAY_1_PIN_ON          GPIO_Pin_3      ///< Пин управления вкелючением для реле 1
#define RELAY_1_PIN_OFF         GPIO_Pin_4      ///< Пин управления выключением для реле 1
#define RELAY_1_PORT_IGBT       GPIOD           ///< Порт управления транзисторами для реле 1
#define RELAY_1_PORT_ON         GPIOD           ///< Порт управления вкелючением для реле 1
#define RELAY_1_PORT_OFF        GPIOD           ///< Порт управления выключением для реле 1

#define RELAY_2_PIN_IGBT        GPIO_Pin_8      ///< Пин управления транзисторами для реле 2
#define RELAY_2_PIN_ON          GPIO_Pin_5      ///< Пин управления вкелючением для реле 2
#define RELAY_2_PIN_OFF         GPIO_Pin_6      ///< Пин управления выключением для реле 2
#define RELAY_2_PORT_IGBT       GPIOB           ///< Порт управления транзисторами для реле 2
#define RELAY_2_PORT_ON         GPIOB           ///< Порт управления вкелючением для реле 2
#define RELAY_2_PORT_OFF        GPIOB           ///< Порт управления выключением для реле 2

#define RELAY_3_PIN_IGBT        GPIO_Pin_3      ///< Пин управления транзисторами для реле 3
#define RELAY_3_PIN_ON          GPIO_Pin_1      ///< Пин управления вкелючением для реле 3
#define RELAY_3_PIN_OFF         GPIO_Pin_2      ///< Пин управления выключением для реле 3
#define RELAY_3_PORT_IGBT       GPIOE           ///< Порт управления транзисторами для реле 3
#define RELAY_3_PORT_ON         GPIOE           ///< Порт управления вкелючением для реле 3
#define RELAY_3_PORT_OFF        GPIOE           ///< Порт управления выключением для реле 3

#define RELAY_4_PIN_IGBT        GPIO_Pin_1      ///< Пин управления транзисторами для реле 4
#define RELAY_4_PIN_ON          GPIO_Pin_2      ///< Пин управления вкелючением для реле 4
#define RELAY_4_PIN_OFF         GPIO_Pin_0      ///< Пин управления выключением для реле 4
#define RELAY_4_PORT_IGBT       GPIOD           ///< Порт управления транзисторами для реле 4
#define RELAY_4_PORT_ON         GPIOD           ///< Порт управления вкелючением для реле 4
#define RELAY_4_PORT_OFF        GPIOD           ///< Порт управления выключением для реле 4

#define RELAY_5_PIN_IGBT        GPIO_Pin_7      ///< Пин управления транзисторами для реле 5
#define RELAY_5_PIN_ON          GPIO_Pin_4      ///< Пин управления вкелючением для реле 5
#define RELAY_5_PIN_OFF         GPIO_Pin_5      ///< Пин управления выключением для реле 5
#define RELAY_5_PORT_IGBT       GPIOD           ///< Порт управления транзисторами для реле 5
#define RELAY_5_PORT_ON         GPIOB           ///< Порт управления вкелючением для реле 5
#define RELAY_5_PORT_OFF        GPIOD           ///< Порт управления выключением для реле 5

#define RELAY_6_PIN_IGBT        GPIO_Pin_9      ///< Пин управления транзисторами для реле 6
#define RELAY_6_PIN_ON          GPIO_Pin_0      ///< Пин управления вкелючением для реле 6
#define RELAY_6_PIN_OFF         GPIO_Pin_7      ///< Пин управления выключением для реле 6
#define RELAY_6_PORT_IGBT       GPIOB           ///< Порт управления транзисторами для реле 6
#define RELAY_6_PORT_ON         GPIOE           ///< Порт управления вкелючением для реле 6
#define RELAY_6_PORT_OFF        GPIOB           ///< Порт управления выключением для реле 6

#define RELAY_BYPAS_PIN_1       GPIO_Pin_7      ///< Пин управления вкелючением для реле 6
#define RELAY_BYPAS_PIN_2       GPIO_Pin_8      ///< Пин управления выключением для реле 6
#define RELAY_BYPAS_PORT_1      GPIOC           ///< Порт управления вкелючением для реле 6
#define RELAY_BYPAS_PORT_2      GPIOC           ///< Порт управления выключением для реле 6

#define ADC_ZIRO                1987             ///< Уровень ноля

#define BUT1 					GPIO_Pin_9		///<Кнопка переключения приоритетов
#define BUT1_PORT				GPIOE			///<Порт управления кнопкой приоритета
void RELAY_Init( void );





#endif /* RELAY_H_ */
