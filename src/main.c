
/*-----�������--------- */

#include "main.h"

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

short flag_aktiv_channel = 0;				// 1 - ����� �; 0 - ��� ����������; 2 - ����� �.
short flag_status_chann_A = 0;				// ��������� ������ �;  0 - ����� ����, 1 - ����� �� � ����.
short flag_status_chann_B = 0;				// ��������� ������ B;  0 - ����� ����, 1 - ����� �� � ����.
short flag_switch_A = 0;		    		// 0 - ���; 1 - ����
short flag_switch_B = 0;		    		// 0 - ���; 1 - ����

short flag_mov_sin_A = 0;						    // 0 -���� �����, 1 -���� ����
short flag_mov_sin_B = 0;						    // 0 -���� �����, 1 -���� ����


//------������� ��������� � ��������� ����
//������������� �������������� �� ��������� �
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //������ ������ ������ �� ������ ������     */

double SIN_A_ref[] = {13,5, 66, 116,5, 164,5, 208,5, 247, 279,5, 305,5, 323,5, 333,5, 335,5,
						329,5, 315,5, 293,5, 264, 228,5, 187, 141, 91,5, 40};			// ����� ������������� ��������� ���������

double SIN_B_ref[] = {-297,5,	-318,5, -331,5, -336, -333, -321, -301,5, -274,5, -241,
						-201,5, -157, -108,5, -57, -4,5, 49, 100, 149, 194, 234,5, 269,5};			// ����� �� ��������� �


double SIN_C_ref[] = {284,5, 253, 215, 172,5, 75, 22,5, -31, -83, -133, -179,5, -222, -258,5,
							-289, -312, -328, -335,5,  -335, -326, -309};						// ����� �� ��������� �


//���������� ������� �� ��������� 10%, 15%, 20%

double shift10 = 24;
double shift15 = 36;
double shift20 = 48;


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
short count_work_A_err = 0;  //������� ������ �������������
short err_flag_sinch_A = 0;	//o - ok; 1- ������ ���� ������ �������������
short count_work_B_err = 0;  //������� ������ �������������
short err_flag_sinch_B = 0;	//o - ok; 1- ������ ���� ������ �������������
short flag_sinch_chan_A = 0;					// 0-��� �������������; 1-���� �������������
short flag_sinch_chan_B = 0;					// 0-��� �������������; 1-���� �������������


void InitSynchA(){
	count_work_A_err = 0;
	err_flag_sinch_A = 0;
	count_work_B_err = 0;
	err_flag_sinch_B = 0;

}
//������� ������� �������������
void SynchA (double *vol){

	if (flag_sinch_chan_A == 0){
		if ((REF_MIN < vol[0]) && (vol[0] < REF_MAX)){
			flag_sinch_chan_A = 1;
			err_flag_sinch_A = 0;
			StartGTimer(GTIMER4);
		}
		else{
			count_work_A_err++;
			if(count_work_A_err < COUN_SINCH_ERR) err_flag_sinch_A = 1;
		}
	}

	if (flag_sinch_chan_B == 0){
			if ((REF_MIN < vol[3]) && (vol[3] < REF_MAX)){
				flag_sinch_chan_B = 1;
				err_flag_sinch_B = 0;
				StartGTimer(GTIMER5);
			}
			else{
				count_work_B_err++;
				if(count_work_B_err < COUN_SINCH_ERR) err_flag_sinch_B = 1;
			}
		}

}
/*������� ����������� ��������� ������*/

void SinQuadrant(int *x, int *y, double *buffA, double *buffB){
	int tmpA = *x;
	int tmpB = *y;
	if(flag_sinch_chan_A == 1){
		tmpA--;
		if(buffA[tmpA] > ZIRO){
			flag_mov_sin_A = 0;
		}
		else{
			flag_mov_sin_A = 1;
		}
	}
	if(flag_sinch_chan_B == 1){
		tmpB--;
		if(buffB[tmpB] > ZIRO){
			flag_mov_sin_B = 0;
		}
		else{
			flag_mov_sin_B = 1;
		}
	}

}



/*������� ������� �������*/
void Freq(){

}


/*������� ������� ������������������� ��������*/
void TrueRMS(){

}


int k = 0;                                  // ������� ��������� �� ����
int k0 = 0;									// ���������� ��� �������� ���� ������� ��������� ������


short int flag_channel_A[3]={0};				//[0] - ���� ��������� ��  0 - ������; 1 - ������;
												//[1] - ���� ��������� AB
												//[2] - ���� ��������� AC

//---------- ������� ��������� ������ ������ A-----------------------------------------------------------
/// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.



void sin_compar_A(double  *vol){
	if (k0 == k) k = 0;
	if (k == 0){
		k = sizeof(SIN_A_ref)/sizeof(double);
		k0 = 0;
	}


/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_A == 0) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref[k0]-shift10) < vol[0] && (SIN_A_ref[k0]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
			}
		}
		else{
			flag_channel_A[0] = 1;
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
			}
		}
		//---------------------------------------------AB------------------------------------------------------
		if((SIN_B_ref[k0]-shift10) < vol[1] && (SIN_B_ref[k0]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//---------------------------------------------AC------------------------------------------------------
		if((SIN_C_ref[k0]-shift10) < vol[2] && (SIN_C_ref[k0]+shift10) > vol[2]) {
			flag_channel_A[2] = 0;
		}
		else{
			flag_channel_A[2] = 1;
		}
	}
/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_A == 1) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if(((SIN_A_ref[k0]*-1)-shift10) < vol[0] && ((SIN_A_ref[k0]*-1)+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k0++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		else{
			flag_channel_A[0] = 1;
			k0++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		//-----------------------------------------------AB----------------------------------------------------
		if(((SIN_B_ref[k0]*-1)-shift10) < vol[1] && ((SIN_A_ref[k0]*-1)+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//-----------------------------------------------AC----------------------------------------------------
		if(((SIN_C_ref[k0]*-1)-shift10) < vol[2] && ((SIN_C_ref[k0]*-1)+shift10) > vol[2]){
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
int kb0 = 0;

short int flag_channel_B[3]={0};				//[0] - ���� ��������� B�   0 - ������; 1 - ������;
												//[1] - ���� ��������� BB
												//[2] - ���� ��������� BC



//--------- ������� ��������� ������ ������ A------------------------------------------------------------
void sin_compar_B(double  *vol){
	if (kb0 == kb) kb = 0;
	if (kb == 0){
		kb = sizeof(SIN_A_ref)/sizeof(double);
		kb0 = 0;
	}


/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_B == 0) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if((SIN_A_ref[kb0]-shift10) < vol[3] && (SIN_A_ref[kb0]+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
			}
		}
		else{
			flag_channel_B[0] = 1;
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
			}
		}
		//---------------------------------------------BB------------------------------------------------------
		if((SIN_B_ref[kb0]-shift10) < vol[4] && (SIN_B_ref[kb0]+shift10) > vol[4]){
			flag_channel_B[1] = 0;
		}
		else{
			flag_channel_B[1] = 1;
		}
		//---------------------------------------------BC------------------------------------------------------
		if((SIN_C_ref[kb0]-shift10) < vol[5] && (SIN_C_ref[kb0]+shift10) > vol[5]) {
			flag_channel_B[2] = 0;
		}
		else{
			flag_channel_B[2] = 1;
		}
	}
/*��������� ��������� ������������� ���������*/
	if ((flag_mov_sin_B == 1) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if(((SIN_A_ref[kb0]*-1)-shift10) < vol[3] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
			}
		}
		else{
			flag_channel_B[0] = 1;
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
			}
		}
		//-----------------------------------------------BB----------------------------------------------------
		if(((SIN_B_ref[kb0]*-1)-shift10) < vol[4] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[4]){
			flag_channel_B[1] = 0;
		}
		else{
			flag_channel_B[1] = 1;
		}
		//-----------------------------------------------BC----------------------------------------------------
		if(((SIN_C_ref[kb0]*-1)-shift10) < vol[5] && ((SIN_C_ref[kb0]*-1)+shift10) > vol[5]){
			flag_channel_B[2] = 0;
		}
		else{
			flag_channel_B[2] = 1;
		}
	}

}



int main(void){



// ������������� ���������

	SYSTEM_Init();
	RELAY_Init();
	ADC_DMA_Init();
	TIMER_Init();
	RS232_Init();
	InitGPIO();
	InitGTimers();  // �������������� ���������� �������
	InitKey();		//������������� ������� ������������ (����������)
	InitSynchA();

	while(1){

				/*ADC_SoftwareStartConv(ADC1);
	            adc_value = ADC_GetConversionValue(ADC1);
	            sprintf(buffer, "%d\r\n", adc_value);
	            USARTSend(buffer, sizeof(buffer));*/

		channel_status();
		switch_channel();
		channel_A_ON();
		channel_B_ON();
		channel_A_OFF();
		channel_B_OFF();
		ButControl();
	}
}











