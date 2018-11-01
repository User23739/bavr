#include "main.h"


/*---Глобальная переменная---- */
 uint16_t ADCBuffer[7] = {0};




void ADC_DMA_Init( void ){

		///////////////////////////////////////////////
		DMA_DeInit(DMA2_Stream0);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	  DMA_InitTypeDef dma2_setup;

	  dma2_setup.DMA_Channel = DMA_Channel_0;			       //channel0 привязан к АЦП1
	  dma2_setup.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;            //копируем из АЦП1
	  dma2_setup.DMA_Memory0BaseAddr = (uint32_t) &ADCBuffer[0];          //адрес буфера, т.е. куда копировать
	  dma2_setup.DMA_BufferSize = 7; //sizeof(adc_buffer);   	       //размер буфера
	  dma2_setup.DMA_DIR = DMA_DIR_PeripheralToMemory;  	               //из периферии в память
	  dma2_setup.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //не изменяем адрес периферии, все время копируем только из АЦП1
	  dma2_setup.DMA_MemoryInc = DMA_MemoryInc_Enable;	               //а вот адрес памяти изменяем, чтобы каждое новое значение записывалось в след. элемент массива
	  dma2_setup.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //размер данных буфера 16бит
	  dma2_setup.DMA_Mode = DMA_Mode_Circular;		               //циклический режим
	  dma2_setup.DMA_Priority = DMA_Priority_High;		               //высший приоритет
	  dma2_setup.DMA_FIFOMode = DMA_FIFOMode_Disable;		       //фифо не используем
	  dma2_setup.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;           //относится к фифо
	  dma2_setup.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //размер данных периферии 16бит
	  dma2_setup.DMA_MemoryBurst = DMA_MemoryBurst_INC4; 	               //относится к пакетной пересылке
	  dma2_setup.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
	  DMA_Init(DMA2_Stream0, &dma2_setup);

	  DMA_Cmd(DMA2_Stream0, ENABLE);
	  //////////////////////////////////////////////////

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE );

	  	GPIO_InitTypeDef pa_setup;
	  	pa_setup.GPIO_Mode = GPIO_Mode_AN;
	  	pa_setup.GPIO_OType = GPIO_OType_PP;
	  	pa_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	  	pa_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  	pa_setup.GPIO_Speed = GPIO_Speed_50MHz;

	  	GPIO_Init(GPIOA, &pa_setup);
	  	/////////////////////////////////////////////////////////////////////////////////////////////
	  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	  	GPIO_InitTypeDef pb_setup;
	  	pb_setup.GPIO_Mode = GPIO_Mode_AN;
	  	pb_setup.GPIO_OType = GPIO_OType_PP;
	  	pb_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	  	pb_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  	pb_setup.GPIO_Speed = GPIO_Speed_50MHz;

	  	GPIO_Init(GPIOB, &pb_setup);
	  	/////////////////////////////////////////////////////
	  	ADC_InitTypeDef ADC_setup;
	  	    ADC_CommonInitTypeDef ADC_CommonInitStructure;

	  	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	  	    ADC_setup.ADC_ScanConvMode = ENABLE;
	  	    ADC_setup.ADC_ContinuousConvMode = ENABLE;
	  	    ADC_setup.ADC_DataAlign = (uint32_t)0x00000000;
	  	    ADC_setup.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	  	    ADC_setup.ADC_NbrOfConversion = 7;  // 1 channel
	  	    ADC_setup.ADC_Resolution = ADC_Resolution_12b;
	  	    ADC_Init(ADC1, &ADC_setup);

	  	    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1,  ADC_SampleTime_3Cycles);
	  	    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2,  ADC_SampleTime_3Cycles);
	  	  	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3,  ADC_SampleTime_3Cycles);
	  	  	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4,  ADC_SampleTime_3Cycles);
	  	  	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5,  ADC_SampleTime_3Cycles);
	  	  	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6,  ADC_SampleTime_3Cycles);
	  	  	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7,  ADC_SampleTime_3Cycles);

	  	    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	  	    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  	    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	  	    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  	    ADC_CommonInit(&ADC_CommonInitStructure);
	  	    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	  	    ADC_DMACmd(ADC1, ENABLE);

	  	    ADC_Cmd(ADC1, ENABLE);
	  	    ADC_SoftwareStartConv(ADC1); // Start ADC1 conversion
	  	}
/*	  	Про пакетную передачу, т.е. то что относится к DMA_MemoryBurst, пока не могу ничего сказать, остальные настройки в комментариях кода.

	  	Оставлю еще пример, настройки DMA для STM32L1, немного отличается, но суть таже.

	  	void init_dma(void)
	  	{
	  		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	  		DMA_InitTypeDef dma_setup;
	  		dma_setup.DMA_BufferSize         =   8;
	  		dma_setup.DMA_DIR                =   DMA_DIR_PeripheralSRC;  //периферия источник(source)
	  	        dma_setup.DMA_M2M                =   DMA_M2M_Disable;
	  		dma_setup.DMA_MemoryBaseAddr     =   (uint32_t)&adc_buf[0];     //source
	  		dma_setup.DMA_MemoryDataSize     =   DMA_MemoryDataSize_HalfWord; //2 bytes
	  		dma_setup.DMA_MemoryInc          =   DMA_MemoryInc_Enable;
	  		dma_setup.DMA_Mode               =   DMA_Mode_Circular;
	  		dma_setup.DMA_PeripheralBaseAddr =   (uint32_t)&(ADC1->DR);
	  		dma_setup.DMA_PeripheralDataSize =   DMA_PeripheralDataSize_HalfWord;
	  		dma_setup.DMA_PeripheralInc      =   DMA_PeripheralInc_Disable;
	  		dma_setup.DMA_Priority           =   DMA_Priority_Medium;

	  		DMA_Init(DMA1_Channel1, &dma_setup);

	  		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //включаем прерывание по половине буфера
	  		DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE); //включаем прерывание по окончанию буфера

	  	        DMA_Cmd(DMA1_Channel1, ENABLE);

	  		NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
	/*------Инизиализация DMA-------------------------------------------------------------*/
	/*DMA_DeInit(DMA2_Stream0);

    DMA_InitTypeDef         DMA_InitStructure;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2,  ENABLE );

    DMA_InitStructure.DMA_Channel               = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t) &(ADC1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr       = (uint32_t) &ADCBuffer[0];
    DMA_InitStructure.DMA_BufferSize            = 1;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular;
    //DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    //DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
   // DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;

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
	/*RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_DeInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE );

	GPIO_InitTypeDef pa_setup;
	pa_setup.GPIO_Mode = GPIO_Mode_AN;
	//pa_setup.GPIO_OType = GPIO_OType_PP;
	pa_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	//pa_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//pa_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &pa_setup);
	/////////////////////////////////////////////////////////////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef pb_setup;
	pb_setup.GPIO_Mode = GPIO_Mode_AN;
	//pb_setup.GPIO_OType = GPIO_OType_PP;
	pb_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	//pb_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//pb_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &pb_setup);
	////////////////////////////////////////////////////////////////////////////////////////////
	/*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef pc_setup;
	pc_setup.GPIO_Mode = GPIO_Mode_AIN;
	pc_setup.GPIO_OType = GPIO_OType_PP;
	pc_setup.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
	pc_setup.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pc_setup.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &pc_setup);
*/
	/*-----------------Инициализация АЦП1---------------------------------------------------*/

	/*ADC_InitTypeDef ADC_setup;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;



	ADC_setup.ADC_ScanConvMode =  DISABLE;
	ADC_setup.ADC_ContinuousConvMode = DISABLE;
	ADC_setup.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_setup.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_setup.ADC_NbrOfConversion = 1;  // 1 channel
	ADC_setup.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &ADC_setup);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6,  ADC_SampleTime_15Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7,  ADC_SampleTime_15Cycles);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
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
*/












