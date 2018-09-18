#include "main.h"





void RS232_Init( void )
{
    USART_InitTypeDef   USART_InitStructure;
    GPIO_InitTypeDef    GPIO_InitStruct;
    NVIC_InitTypeDef    NVIC_InitStructure;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}


