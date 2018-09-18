#include "main.h"



//�������������

extern short flag_switch_A;		    		// 0 - ���; 1 - ����
extern short flag_switch_B;
extern short flag_aktiv_channel;



void RELAY_Init( void ){

	GPIO_InitTypeDef    GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	//1 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_1_PIN_IGBT | RELAY_1_PIN_ON | RELAY_1_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//2 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_2_PIN_IGBT | RELAY_2_PIN_ON | RELAY_2_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//3 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_3_PIN_IGBT | RELAY_3_PIN_ON | RELAY_3_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//4 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_4_PIN_IGBT | RELAY_4_PIN_ON | RELAY_4_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);


	//5 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_5_PIN_IGBT | RELAY_5_PIN_ON | RELAY_5_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//6 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_6_PIN_IGBT | RELAY_6_PIN_ON | RELAY_6_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOE, &GPIO_InitStruct);

}

//------������� ������������� ������� ������--------------------------------------------------------------

void InitKey(void){
	/*��������� ����������� */
	//1
	GPIO_ResetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
	//2
	GPIO_ResetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
	//3
	GPIO_ResetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
	//4
	GPIO_ResetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
	//5
	GPIO_ResetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
	//6
	GPIO_ResetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
	/*��������� 1  ���� */
	GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
	GPIO_SetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
	/*��������� 2 ���� */
	GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
	GPIO_SetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
	/*��������� 3  ���� */
	GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
	GPIO_SetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
	/*��������� 4  ���� */
	GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
	GPIO_SetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
	/*��������� 5  ���� */
	GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
	GPIO_SetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
	/*��������� 6  ���� */
	GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
	GPIO_SetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
	/*��� ���� ������� �� ����� ������� �������� ������� ���������� ���� 15 ��*/
	StartGTimer(GTIMER3);
	flag_aktiv_channel = 0;			//���������� ���� ���������� ����� �������
}

//-------������� ��������� ������ �---------------------------------------------------------------------

void channel_A_ON(void){
	unsigned int Timer2 = 0;
	unsigned int Timer3 = 0;
	if (flag_switch_A == 0){       //���� � �������� ��� ��������� ������ �
		switch(flag_aktiv_channel){
			case 0:
				Timer3 = GetGTimer(GTIMER3);
				if(Timer3 >KEY_DELAY){
				/*��������� ����������� */
					//1
					GPIO_SetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
					//2
					GPIO_SetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
					//3
					GPIO_SetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
					flag_aktiv_channel = 1;

				/*�������� ����*/
					//1
					GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
					GPIO_SetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
					//2
					GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
					GPIO_SetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
					//3
					GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
					GPIO_SetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
					StopGTimer(GTIMER3);
					Timer3 = 0;
				}

				break;
			case 1:
				/*��� ������ �� ������ � ������ �������*/
				break;
			case 2:
					Timer2 = GetGTimer(GTIMER2);
					if(Timer2 > KEY_DELAY){
						/*��������� ����������� */
						//1
						GPIO_SetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
						//2
						GPIO_SetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
						//3
						GPIO_SetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
						flag_aktiv_channel = 1;

					/*�������� ����*/
						//1
						GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
						GPIO_SetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
						//2
						GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
						GPIO_SetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
						//3
						GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
						GPIO_SetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
						StopGTimer(GTIMER2);
						Timer2 = 0;

					}

				break;
		}
	}
	else{
		/*��� �������� �������������� ����������*/
	}


}

//-------������� ��������� ������ B---------------------------------------------------------------------

void channel_B_ON(void){
	unsigned int Timer1 = 0;
	unsigned int Timer3 = 0;
	if (flag_switch_B == 0){
			switch(flag_aktiv_channel){
				case 0:
					Timer3 = GetGTimer(GTIMER3);
					if(Timer3 >KEY_DELAY){
					/*��������� ����������� */
						//4
						GPIO_SetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
						//5
						GPIO_SetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
						//6
						GPIO_SetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
						flag_aktiv_channel = 2;


					/*�������� ����*/
						//4
						GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
						GPIO_SetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
						//5
						GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
						GPIO_SetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
						//6
						GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
						GPIO_SetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
						StopGTimer(GTIMER3);
						Timer3 = 0;
					}


					break;
				case 1:
						Timer1 = GetGTimer(GTIMER1);
						if(Timer1 >KEY_DELAY){
							/*��������� ����������� */
							//4
							GPIO_SetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
							//5
							GPIO_SetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
							//6
							GPIO_SetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
							flag_aktiv_channel = 2;

						/*�������� ����*/
							//4
							GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
							GPIO_SetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
							//5
							GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
							GPIO_SetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
							//6
							GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
							GPIO_SetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
							StopGTimer(GTIMER1);
							Timer1 = 0;
						}

						break;
				case 2:
					/*��� ������ �� ������*/
					   break;
			}
		}
		else{
				/*��� �������� �������������� ����������*/
		}



}

//-------������� ���������� ������ �---------------------------------------------------------------------

void channel_A_OFF(void){
	if (flag_switch_A == 1){
	/*��������� ����������� */
		//1
		GPIO_ResetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
		//2
		GPIO_ResetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
		//3
		GPIO_ResetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
		/*��������� 1  ���� */
		GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
		GPIO_SetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
		/*��������� 2 ���� */
		GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
		GPIO_SetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
		/*��������� 3  ���� */
		GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
		GPIO_SetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
		/*��� ���� ������� �� ����� ������� �������� ������� ���������� ���� 15 ��*/
		StartGTimer(GTIMER1);
	}




}

//-------������� ���������� ������ B---------------------------------------------------------------------

void channel_B_OFF(void){
	if (flag_switch_B == 1){
	/*��������� ����������� */
	//4
	GPIO_ResetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
	//5
	GPIO_ResetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
	//6
	GPIO_ResetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
	/*��������� 4  ���� */
	GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
	GPIO_SetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
	/*��������� 5  ���� */
	GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
	GPIO_SetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
	/*��������� 6  ���� */
	GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
	GPIO_SetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
	/*��� ���� ������� �� ����� ������� �������� ������� ���������� ���� 15 ��*/
	StartGTimer(GTIMER2);
	}

}

/* ��������� ������ ������ ������
 * 1. ���������� / ������������ (������ ���������, � ����� ������ ������������� �� ������)
 * 2. ���������� / ����������� ��� ������� �� ������� (���� � ����)(������ ��������� �� ����� �� �������� ��� ������� ����)
 * 3. ������������ / ������������ (������ �� ���������, � ����� ������ ���������� �� ������)
 * 4. ������������ / ���������� ��� ����� �� ������� (������ �� ���������, �� ����� �� �������� ��� ������� ����)*/



