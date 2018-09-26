
/*-----�������--------- */

#include "main.h"




//��������� ������������ � �������� 1 ��.


/*-----------������-------------------------------------------------------*/
/*-----��������� �������--------------------------------------------------*/





//-------����������� � ������� ��� ������------------------------------------
volatile char buffer[20] = {'\0'};  // ����� ��� �������� ������, �����������



void USARTSend(const unsigned char *pucBuffer){
	while (*pucBuffer)
	{
		USART_SendData(USART1, *pucBuffer++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
		}
	}
}


//--���������� ����������
extern uint32_t buff_chanA1[];
extern uint32_t buff_chanB1[];
extern int a1;
extern int b1;

short flag_aktiv_channel = 0;				// 1 - ����� �; 0 - ��� ����������; 2 - ����� �.
short flag_status_chann_A = 0;				// ��������� ������ �;  0 - ����� ����, 1 - ����� �� � ����.
short flag_status_chann_B = 0;				// ��������� ������ B;  0 - ����� ����, 1 - ����� �� � ����.
short flag_switch_A = 0;		    		// 0 - ���; 1 - ����
short flag_switch_B = 0;		    		// 0 - ���; 1 - ����

//------������� ��������� � ��������� ����
//������������� �������������� �� ��������� �
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //������ ������ ������ �� ������ ������     */

uint32_t SIN_A_ref_up[] = {2048, 2152, 2255, 2356, 2455, 2552, 2646, 2737, 2823, 2904, 2980,
		3050, 3114, 3171, 3222, 3265, 3301, 3329, 3349, 3361, 3365, 3361, 3349, 3329, 3301,
		3265, 3222, 3171, 3114, 3050, 2980, 2904, 2823, 2737, 2646, 2552, 2455, 2356, 2255, 2152, 2048};			// ����� ������������� ��������� ���������

uint32_t SIN_A_ref_dw[] = {1983, 1577, 1209, 918, 731, 666, 731, 918, 1209, 1577 };			// ����� ������������� ��������� ���������

uint32_t SIN_C_ref_up[] = {3124, 2865, 2520, 2122, 1710, 1325, 1005, 781, 674, 695 };			// ����� ������������� ��������� ��������� A
uint32_t SIN_C_ref_dw[] = {843, 1102, 1447, 1845, 2257, 2642, 2962, 3186, 3293, 3272 };			// ����� ������������� ��������� ��������� A

uint32_t SIN_B_ref_up[] = {843,	695, 674, 780, 1005, 1325, 1710, 2121, 2519, 2865 };			// ����� ������������� ��������� ��������� A
uint32_t SIN_B_ref_dw[] = {3124, 3272, 3293, 3187, 2962, 2642, 2257, 1846, 1448, 1102 };			// ����� ������������� ��������� ��������� A




//���������� ������� �� ��������� 10%, 15%, 20%

uint32_t shift10 = 132;
uint32_t shift15 = 198;
uint32_t shift20 = 264;


//-------------------------




void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
    	Aver();						//��������� ������� ����������
    	ProcessTimers();			//�������� ������� ���������� �������� �������
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

/*
 ����� 0 � ������������� */
/*	if (flag_sinch_chan_A == 0)	{
		if((SIN_A_ref_up[0]-shift10) < vol[0] && (SIN_A_ref_up[0]+shift10) > vol[0]) {
			flag_sinch_chan_A =1;
			k = 0;
		}
		else{
			flag_sinch_chan_A = 0;
		}
	}
*/


/*������� ������������� */
//������������� ������� �������������
short count_work_err = 0;  //������� ������ �������������
short err_flag_sinch = 0;	//o - ok; 1- ������ ���� ������ �������������
short flag_sinch_chan_A = 0;					// 0-��� �������������; 1-���� �������������
short flag_sinch_chan_B = 0;					// 0-��� �������������; 1-���� �������������


void InitSynchA(){
	count_work_err = 0;
	err_flag_sinch = 0;

}
//������� ������� �������������
void SynchA (double *vol){

	if (flag_sinch_chan_A == 0){
		if ((REF_MIN < vol[0]) && (vol[0] > REF_MAX)){
			flag_sinch_chan_A = 1;
			 err_flag_sinch = 0;
			StartGTimer(GTIMER4);
		}
		else{
			count_work_err++;
			if(count_work_err < COUN_SINCH_ERR) err_flag_sinch = 1;
		}
	}


}
/*������� ����������� ��������� ������*/

void SinQuadrant(){







	/*a1l = a1-1;
		if ((k == 0) && (flag_sinch_chan_A == 1)){

			if (buff_chanA1[a1l] > SIN_A_ref_up[0]){
					flag_mov_sin_A = 1;
			}
			else{
					flag_mov_sin_A = 0;
			}
		}*/
}

/*������� ������� �������*/
void Freq(){

}


/*������� ������� ������������������� ��������*/
void TrueRMS(){

}


int k = 0;                                  // ������� ��������� �� ����
int a1l=0;									// ���������� ��� �������� ���� ������� ��������� ������

short flag_mov_sin_A = 0;						    // 0 -���� �����, 1 -���� ����
short int flag_channel_A[3]={0};				//[0] - ���� ��������� ��  0 - ������; 1 - ������;
												//[1] - ���� ��������� AB
												//[2] - ���� ��������� AC

//---------- ������� ��������� ������ ������ A-----------------------------------------------------------
/// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.



void sin_compar_A(double  *vol){
	if (k == 10) k = 0;



/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_A == 0) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref_up[k]-shift10) < vol[0] && (SIN_A_ref_up[k]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		else{
			flag_channel_A[0] = 1;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		//---------------------------------------------AB------------------------------------------------------
		if((SIN_B_ref_up[k]-shift10) < vol[1] && (SIN_B_ref_up[k]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//---------------------------------------------AC------------------------------------------------------
		if((SIN_C_ref_up[k]-shift10) < vol[2] && (SIN_C_ref_up[k]+shift10) > vol[2]) {
			flag_channel_A[2] = 0;
		}
		else{
			flag_channel_A[2] = 1;
		}
	}
/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_A == 1) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref_dw[k]-shift10) < vol[0] && (SIN_A_ref_dw[k]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		else{
			flag_channel_A[0] = 1;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		//-----------------------------------------------AB----------------------------------------------------
		if((SIN_B_ref_dw[k]-shift10) < vol[1] && (SIN_B_ref_dw[k]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//-----------------------------------------------AC----------------------------------------------------
		if((SIN_C_ref_dw[k]-shift10) < vol[2] && (SIN_C_ref_dw[k]+shift10) > vol[2]){
			flag_channel_A[2] = 0;
		}
		else{
			flag_channel_A[2] = 1;
		}
	}

}
//������� ��������� ��������� ������ B

// ���������� ��������
int kb = 0;                                  // ������� ��������� �� ����

short flag_mov_sin_B = 0;						    // 0 -���� �����, 1 -���� ����
short int flag_channel_B[3]={0};				//[0] - ���� ��������� B�   0 - ������; 1 - ������;
												//[1] - ���� ��������� BB
												//[2] - ���� ��������� BC



//--------- ������� ��������� ������ ������ A------------------------------------------------------------
void sin_compar_B(double *vol){  /// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.
	//������� ������� �������� �� �� 10 ��������
	if (kb < 10){
		kb = 0;
		flag_sinch_chan_B =0;
	}

	after_B = befor_B;
	befor_B = vol[3];

	if (after_B !=0){		// ������ ���� �� ���������� ��������

		if (kb == 0){				// �������� �� �� ����� ������� ����

			if((SIN_A_ref_up[kb]-shift10) < vol[3] && (SIN_A_ref_up[kb]+shift10) > vol[3]){   //�������������� 0 � ������� ������
				flag_sinch_chan_B =1;
						// ���������� ���������� ����������� ���������.
					if (after_B > SIN_A_ref_up[kb])	{
							flag_mov_sin_B = 1;
							kb++;
						}
					else{
							flag_mov_sin_B = 0;
							kb++;
						}
				}
			else{
				flag_sinch_chan_B =0;
			}
		}
		else{		// ���� ��� ��� ������ ����
			// ���� ��� ������� ����������� �������� �� ���������
			if (flag_mov_sin_B == 0){  //��� ������� �������� �������� � ��� � ���� ������� ��������
				//----------------------------------------------BA1-----------------------------------------------------
				if((SIN_A_ref_up[k]-shift10) < vol[3] && (SIN_A_ref_up[k]+shift10) > vol[3]){     // ���������� ���������
					flag_channel_B[0] = 0;
					kb++;
				}
				else{
					flag_channel_B[0] = 1;
					kb++;
				}
				//---------------------------------------------BB1------------------------------------------------------
				if((SIN_B_ref_up[k]-shift10) < vol[4] && (SIN_B_ref_up[k]+shift10) > vol[4]){     // ���������� ���������
					flag_channel_B[1] = 0;
				}
				else{
					flag_channel_B[1] = 1;
				}
				//---------------------------------------------BC1------------------------------------------------------
				if((SIN_C_ref_up[k]-shift10) < vol[5] && (SIN_C_ref_up[k]+shift10) > vol[5]){     // ���������� ���������
					flag_channel_B[2] = 0;
				}
				else{
					flag_channel_B[2] = 1;
				}
			}
			else{
				//----------------------------------------------BA1-----------------------------------------------------
				if((SIN_A_ref_dw[k]-shift10) < vol[3] && (SIN_A_ref_dw[k]+shift10) > vol[3]){
					flag_channel_B[0] = 0;
					k++;
				}
				else{
					flag_channel_B[0] = 1;
					k++;
				}
				//-----------------------------------------------BB1----------------------------------------------------
				if((SIN_B_ref_dw[k]-shift10) < vol[4] && (SIN_B_ref_dw[k]+shift10) > vol[4]){
					flag_channel_B[1] = 0;
				}
				else{
					flag_channel_B[1] = 1;
				}
				//-----------------------------------------------BC1----------------------------------------------------
				if((SIN_C_ref_dw[k]-shift10) < vol[5] && (SIN_C_ref_dw[k]+shift10) > vol[5]){
					flag_channel_B[2] = 0;
				}
				else{
					flag_channel_B[2] = 1;
				}
			}
		}
	}
}



int main(void){



// ������������� ���������

	SYSTEM_Init();
	//RELAY_Init();
	ADC_DMA_Init();
	TIMER_Init();
	RS232_Init();
	//InitGPIO();
	InitGTimers();  // �������������� ���������� �������
	//InitKey();		//������������� ������� ������������ (����������)
	InitSynchA();

	while(1){

				/*ADC_SoftwareStartConv(ADC1);
	            adc_value = ADC_GetConversionValue(ADC1);
	            sprintf(buffer, "%d\r\n", adc_value);
	            USARTSend(buffer, sizeof(buffer));*/

		//channel_status();
		//switch_channel();
		//channel_A_ON();
		//channel_B_ON();
		//channel_A_OFF();
		//channel_B_OFF();
		//ButControl();
	}
}
