#include "main.h"









void TIMER_Init(void)

{
	TIMER_Init_4();
	TIMER_Init_3();




}



void TIMER_Init_4(void)

{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		//****************TIM4 for 1 tick = 1uS

  		//5000 мкс
	    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	    TIM_TimeBaseStructure.TIM_Prescaler= 49;
	    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	    TIM_TimeBaseStructure.TIM_Period = 250;//до этого значения будет считать таймер 250 uS/ 250 мкС
	    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	    TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);



	    //TIM_Cmd(TIM4, ENABLE);

	    // Настройка прерывания
	    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn  ;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);

	    TIM_Cmd(TIM4, ENABLE); //пока не разрешаем работу



}

void TIMER_Init_3(void)

{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		//****************TIM4 for Modbus 1 tick = 1uS

  		//15000 мкс
	    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	    TIM_TimeBaseStructure.TIM_Prescaler= 49;
	    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	    TIM_TimeBaseStructure.TIM_Period = 4000;//до этого значения будет считать таймер
	    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);





	    // Настройка прерывания
	    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn  ;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);

	    //TIM_Cmd(TIM3, ENABLE); //пока не разрешаем работу



}
