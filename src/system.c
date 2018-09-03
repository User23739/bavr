/*
 * system.c
 *
 *  Created on: 27 июл. 2018 г.
 *      Author: a.boyarkin
 */



/**----- Инклуды -----------------------------------------------------------------------------------------*/
#include "system.h"
#include "stm32f2xx_rcc.h"

/**----- Макросы константы -------------------------------------------------------------------------------*/
/**----- Макросы -----------------------------------------------------------------------------------------*/
/**----- Локальные типы данных ---------------------------------------------------------------------------*/
RCC_ClocksTypeDef RCC_temp;
/**----- Локальные переменные ----------------------------------------------------------------------------*/
/**----- Глобальные переменные ---------------------------------------------------------------------------*/
/**----- Прототипы функций -------------------------------------------------------------------------------*/
static void RCC_Configuration(void);


/**----- Глобальные функции ------------------------------------------------------------------------------*/

void SYSTEM_Init (void )
{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;


    RCC_Configuration();


}


static void RCC_Configuration(void)
{

    __IO uint32_t HSEStatus = 0;

    // Cброс настроек RCC
    RCC_DeInit();

    RCC_HSEConfig( RCC_HSE_ON );
    RCC_LSEConfig( RCC_LSE_OFF );

    HSEStatus = RCC_WaitForHSEStartUp();

    if (HSEStatus)
        RCC_PLLConfig(RCC_PLLSource_HSE, 13, 208, 4, 4);
    else
    {
        RCC_HSEConfig( RCC_HSE_OFF );
        RCC_PLLConfig(RCC_PLLSource_HSI, 8, 200, 4, 4);
    }

    RCC_PLLCmd( ENABLE );
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Enable Prefetch Buffer
    FLASH_PrefetchBufferCmd( ENABLE );
    // Цикл ожидания = 3
    FLASH_SetLatency( FLASH_Latency_3 );

    RCC_PCLK1Config( RCC_HCLK_Div4 );
    RCC_PCLK2Config( RCC_HCLK_Div2 );

    RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

    RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2);
    RCC_MCO2Config(RCC_MCO2Source_PLLCLK, RCC_MCO2Div_2);

//    RCC_HSICmd( DISABLE );
//    RCC_LSICmd( DISABLE );
    RCC_ClockSecuritySystemCmd( ENABLE );
    RCC_GetClocksFreq(&RCC_temp);
}



// На слуай если вдруг сдохнет кварц
void NMI_Handler()
{
    RCC_ClearITPendingBit(RCC_IT_CSS);

    RCC_HSEConfig(RCC_HSE_ON);
    if ( RCC_WaitForHSEStartUp() == SUCCESS )
    {
       RCC_PLLConfig(RCC_PLLSource_HSE, 13, 208, 4, 4);
    }
    else
    {
        RCC_HSEConfig( RCC_HSE_OFF );
        RCC_PLLConfig(RCC_PLLSource_HSI, 8, 200, 4, 4);
    }
    RCC_PLLCmd( ENABLE );
}





/**----- Локальные функции -------------------------------------------------------------------------------*/


/**----- КОНЕЦ ФАЙЛА -------------------------------------------------------------------------------------*/

