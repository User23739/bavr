#include "main.h"


//volatile uint16_t ADCBuffer[7] = {0};
 uint32_t ADCBuffer[7] = {0};
 //uint32_t *uk = ADCBuffer;



void ADC_DMA_Init( void )
{
	/*------Инизиализация DMA-------------------------------------------------------------*/
	DMA_DeInit(DMA2_Stream0);

    DMA_InitTypeDef         DMA_InitStructure;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2,  ENABLE );

    DMA_InitStructure.DMA_Channel               = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t) &(ADC1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t) &ADCBuffer[0];
    DMA_InitStructure.DMA_BufferSize            = 7;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = (uint32_t)0x00000400;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);



    /*DMA_InitStructure.DMA_Channel               = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(ADC1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)&(ADCBuffer[0]);
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize            = 7;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);*/

   /* DMA_InitStructure.DMA_Channel               = DMA_Channel_1;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(ADC2->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t)&(data.chanel_data.measure_data[5]);
    DMA_Init(DMA2_Stream3, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream3, ENABLE);*/

	/*-----------Инициализация GPIO--------------------------------------------------------------*/


	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );

	GPIO_InitTypeDef pa_setup;
	pa_setup.GPIO_Mode = GPIO_Mode_AIN;
	pa_setup.GPIO_OType = GPIO_OType_PP;
	pa_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	pa_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pa_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &pa_setup);
	/////////////////////////////////////////////////////////////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef pb_setup;
	pb_setup.GPIO_Mode = GPIO_Mode_AIN;
	pb_setup.GPIO_OType = GPIO_OType_PP;
	pb_setup.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	pb_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pb_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &pb_setup);
	////////////////////////////////////////////////////////////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef pc_setup;
	pc_setup.GPIO_Mode = GPIO_Mode_AIN;
	pc_setup.GPIO_OType = GPIO_OType_PP;
	pc_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
	pc_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pc_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &pc_setup);

	/*-----------------Инициализация АЦП1---------------------------------------------------*/

	ADC_InitTypeDef ADC_setup;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_setup.ADC_ScanConvMode =  ENABLE;
	ADC_setup.ADC_ContinuousConvMode = ENABLE;
	ADC_setup.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_setup.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_setup.ADC_NbrOfConversion = 7;  // 1 channel
	ADC_setup.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &ADC_setup);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6,  ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7,  ADC_SampleTime_15Cycles);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1); // Start ADC1 conversion
}





	/*ADC_InitTypeDef         ADC_InitStructure;
	ADC_CommonInitTypeDef   ADC_CommonInitStructure;

	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,  ENABLE );

    /*GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStruct);*/

    /*ADC_CommonInitStructure.ADC_Mode                = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler           = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode       = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay    = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution                = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode              = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode        = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge      = ADC_ExternalTrigConvEdge_None;
    //ADC_InitStructure.ADC_ExternalTrigConv          = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign                 = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion           = 7;
    ADC_Init(ADC1, &ADC_InitStructure);
    //ADC_Init(ADC2, &ADC_InitStructure);


    //ADC_RegularChannelConfig(ADC2, ADC_Channel_13, 3, ADC_SampleTime_15Cycles);
    //ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 4, ADC_SampleTime_15Cycles);
    //ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 5, ADC_SampleTime_15Cycles);

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
   // ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);

    ADC_DMACmd(ADC1, ENABLE);
    //ADC_DMACmd(ADC2, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
   // ADC_Cmd(ADC2, ENABLE);

    ADC_SoftwareStartConv(ADC1);
    //ADC_SoftwareStartConv(ADC2);

}*/











