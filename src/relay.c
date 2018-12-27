#include "main.h"



//инизиализация

//extern short flag_switch_B;
/*----Глобальные переменные------------*/
short flag_aktiv_channel = 0;				// 0-оба откл; 1-вкл А; 2-вкл В;
//extern short flag_z_switch;
//extern short flag_sinch_ch;
extern short key;


void RELAY_Init( void ){

	GPIO_InitTypeDef    GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	//1 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_1_PIN_IGBT | RELAY_1_PIN_ON | RELAY_1_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//2 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_2_PIN_IGBT | RELAY_2_PIN_ON | RELAY_2_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//3 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_3_PIN_IGBT | RELAY_3_PIN_ON | RELAY_3_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//4 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_4_PIN_IGBT | RELAY_4_PIN_ON | RELAY_4_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);


	//5 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_5_PIN_IGBT | RELAY_5_PIN_ON | RELAY_5_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//6 реле
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_6_PIN_IGBT | RELAY_6_PIN_ON | RELAY_6_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOE, &GPIO_InitStruct);

}

//------функция инициализации силовых ключей--------------------------------------------------------------

void InitKey(void){
	ChannelAOffSS();
	ChannelAOffRelay(TIMER_ON);

	ChannelBOffSS();
	ChannelBOffRelay(TIMER_ON);

	//StartGTimer(GT_INIT_KEY);




	flag_aktiv_channel = 0;							//оба канала отключенны
}

//-------функция включения канала А---------------------------------------------------------------------
void ChannelAOnSS(uint8_t status_timer){
	uint8_t st_timer;

	/*Включаемм транзисторы */
			//1
			GPIO_SetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
			//2
			GPIO_SetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
			//3
			GPIO_SetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
			st_timer = status_timer;
			/// Условия включения таймера
			if(st_timer) StartGTimer(GT_ON_SS_A);
			if(!st_timer)ResetGTimer(GT_ON_SS_A);

			flag_aktiv_channel = 1;

}

void ChannelAOnRelay(uint8_t status_timer){
	uint8_t st_timer;
	/*Включаем реле*/
		//1
		GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
		GPIO_SetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
		//2
		GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
		GPIO_SetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
		//3
		GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
		GPIO_SetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
		st_timer = status_timer;
		/// Условия включения таймера
		if(st_timer) StartGTimer(GT_ON_RELAY_A);

		flag_aktiv_channel = 1;
}

//-------функция включения канала B---------------------------------------------------------------------
void ChannelBOnSS(uint8_t status_timer){
	uint8_t st_timer;
	/*Включаемм транзисторы */
		//4
		GPIO_SetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
		//5
		GPIO_SetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
		//6
		GPIO_SetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
		st_timer = status_timer;
		/// Условия включения таймера
		if(st_timer) StartGTimer(GT_ON_SS_B);
		if(!st_timer)ResetGTimer(GT_ON_SS_B);

		flag_aktiv_channel = 2;

}

void ChannelBOnRelay(uint8_t status_timer){
	uint8_t st_timer;
	/*Включаем реле*/
		//4
		GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
		GPIO_SetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
		//5
		GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
		GPIO_SetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
		//6
		GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
		GPIO_SetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
		st_timer = status_timer;
		/// Условия включения таймера
		if(st_timer) StartGTimer(GT_ON_RELAY_B);


		flag_aktiv_channel = 2;

}

//-------функция отключения канала А---------------------------------------------------------------------
void ChannelAOffSS(void){


	/*Отключаем транзисторы */
			//1
			GPIO_ResetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
			//2
			GPIO_ResetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
			//3
			GPIO_ResetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);


}

void ChannelAOffRelay(uint8_t status_timer){
	uint8_t st_timer;
		/*Отключаем 1  реле */
		GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
		GPIO_SetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
		/*Отключаем 2 реле */
		GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
		GPIO_SetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
		/*Отключаем 3  реле */
		GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
		GPIO_SetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
		st_timer = status_timer;
		/// Условия включения таймера
		if(st_timer) StartGTimer(GT_OFF_RELAY_A);



}

//-------функция отключения канала B---------------------------------------------------------------------
void ChannelBOffSS(void){


	/*Отключаем транзисторы */
		//4
		GPIO_ResetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
		//5
		GPIO_ResetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
		//6
		GPIO_ResetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);

}


void ChannelBOffRelay(uint8_t status_timer){
	uint8_t st_timer;
	/*Отключаем 4  реле */
	GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
	GPIO_SetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
	/*Отключаем 5  реле */
	GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
	GPIO_SetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
	/*Отключаем 6  реле */
	GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
	GPIO_SetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
	st_timer = status_timer;
	/// Условия включения таймера
	if(st_timer) StartGTimer(GT_OFF_RELAY_B);

}

void PulsOffPolRelay(void){
	if (key == KEY_STAT_SW_RELAY ){
		switch(flag_aktiv_channel){
			case ACTIV_OFF: //включение с ноля
				if(GetGTimer(GT_INIT_KEY) >STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канала А */
					GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
					GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
					GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);

					/*Отключаем ипмульс не реле канале В */
					GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
					GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
					GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);

					StopGTimer(GT_INIT_KEY);
				}
				break;
			case ACTIV_CH_A:
				if(GetGTimer(GT_ON_RELAY_A) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канала А */
					GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
					GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
					GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);

					StopGTimer(GT_ON_RELAY_A);
				}

				if(GetGTimer(GT_OFF_RELAY_A) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канале A */
					GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
					GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
					GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);

					StopGTimer(GT_OFF_RELAY_A);
				}
				if(GetGTimer(GT_OFF_RELAY_B) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канале В */
					GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
					GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
					GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);

					StopGTimer(GT_OFF_RELAY_B);
				}



				break;
			case ACTIV_CH_B:
				if(GetGTimer(GT_ON_RELAY_B) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канала B */
					GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
					GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
					GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);

					StopGTimer(GT_ON_RELAY_B);
				}
				if(GetGTimer(GT_OFF_RELAY_B) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канале В */
					GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
					GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
					GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);

					StopGTimer(GT_OFF_RELAY_B);
				}
				if(GetGTimer(GT_OFF_RELAY_A) >=STAT_RELAY_DELAY){
					/*Отключаем ипмульс не реле канале A */
					GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
					GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
					GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);

					StopGTimer(GT_OFF_RELAY_A);
				}



				break;
			default:
				break;
		}
	}
}

void PulsOffStatSwitch(void){
	if (key == KEY_STAT_SW_RELAY ){
			switch(flag_aktiv_channel){
				case ACTIV_OFF: //включение с ноля

					break;
				case ACTIV_CH_A:
					if(GetGTimer(GT_ON_SS_A) >=STAT_SW_DELAY){
						ChannelAOffSS();
						StopGTimer(GT_ON_SS_A);
					}
					break;
				case ACTIV_CH_B:
					if(GetGTimer(GT_ON_SS_B) >=STAT_SW_DELAY){
						ChannelBOffSS();
						StopGTimer(GT_ON_SS_B);
					}
					break;
				default:
					break;
			}
		}
}



/* Возможные режимы работы ключей
 * 1. Синхронный / переключение (каналы синхронны, а нужно просто переключиться на другой)
 * 2. Синхронный / пропаданием или выходом за границы (одно и тоже)(каналы синхронны но вышли за диапазон или пропала сеть)
 * 3. Несинхронный / переключение (каналы не синхронны, а нужно просто перечиться на другой)
 * 4. Несинхронный / пропадание или выход за границы (каналы не синхронны, но вышли за диапазон или пропала сеть)*/



