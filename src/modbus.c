/*
 * modbus.c
 *
 *  Created on: 2 ����. 2018 �.
 *      Author: a.boyarkin
 */

#include "main.h"




unsigned short reg_data[30];

#define MaxLenRecBuf 250//p����� ������ ����������� ������
#define MaxLenTrBuf 250 //p����� ������ ������������ ������

unsigned char cNumRcByte0;//���-�� �������� ���� ������
unsigned char cNumTrByte0;//���-�� ������������ ���� ������

unsigned char RcCount,TrCount;//�������� �������� � ���������� ������

unsigned char cmRcBuf0[MaxLenRecBuf];//����� ����������� ������
unsigned char cmTrBuf0[MaxLenTrBuf];//����� ������������ ������
unsigned int first_reg;
unsigned int kolreg;
int tempCRC;
unsigned char cnt_reg16;

//#include <stdbool.h>

#define TRUE 1
#define FALSE 0


bool bModBus;//���� ���������� ��������� �������
bool StartRec;//���� ������ ������ ������� �� Modbus


//������� ��� ������� crc-16
const unsigned char srCRCHi[256]=
{
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40
};
const unsigned char srCRCLo[256]=
{
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40
};

//******************************************************************************************/
//������� ��������� crc-16                                                                 */
//�� ����� ��������� �� ������ ������ , ����������                                                 */
//�������� ������, ������� ����� �������������� ��� ������� CRC                            */
//� ���������� ������ � ������ ���������.                                                  */
/********************************************************************************************/
int GetCRC16(unsigned char *buf,  char bufsize)
{
	unsigned  char CRC_Low=0xFF;
	unsigned  char CRC_High=0xFF;
	unsigned  char k;
	unsigned   char carry;

        if (bufsize>256U)
        {
                return (0);
        }

        for (k=0; k<bufsize; k++)
        {
        	carry=CRC_Low^buf[k];
        	CRC_Low=CRC_High^srCRCHi[carry];
        	CRC_High=srCRCLo[carry];
        }
        return ((CRC_High << 8) | CRC_Low);
}//end GetCRC16

//������������ ������ �� ������
char ErrorMessage(char Error)
{
	char TempI;
	cmTrBuf0[1]=cmRcBuf0[1]+0x80;//������� � �������
	cmTrBuf0[2]=Error;
	TempI=GetCRC16(cmTrBuf0,3);//������� ����������� ����� �������
	cmTrBuf0[3]=TempI;
    cmTrBuf0[4]=TempI>>8;
	  return 5;
	}//end ErrorMessage()

/********************************************************************************************/
//������������ ��������� �������� �������                                                                    */
/********************************************************************************************/

 char ModBus(unsigned char NumByte)
{
   kolreg=0;
   first_reg=0;
   tempCRC=0;
   unsigned char i_for;
   int CRC16;
   int TempI;
   cmRcBuf0[NumByte]=0x00;

  //��������� �������
  if (cmRcBuf0[0]!=0x02) return 0x00; //����� ����������(������ ���� � �������).
                                          //���� ����� ������������(�� ����� 32), �� ����������
                                          //� ���������� cNumTrByte0 �������� 0.
  CRC16=GetCRC16(cmRcBuf0,NumByte-2); //������� CRC-16 � �������� �������
  TempI=(int) (cmRcBuf0[NumByte-1]<<8) + cmRcBuf0[NumByte-2];
  if (CRC16!=TempI) return 0x00;       //�� �� ���������,����� �� �����
  cmTrBuf0[0]=0x02;//����� ������ ����������=32

//��� �������
  switch(cmRcBuf0[1])  //����������� 2�� ����� � �������.
   {
   case 0x03:{//���� ���� ����� 0x03 �� ���
               //����� ������� Modbus (������ ������ ��������);

	        //��������� ����� ��������� ��� ������
	        kolreg=(int) (cmRcBuf0[4]<<8) + cmRcBuf0[5];
            //����� ������� �������������� ��������
	        first_reg=(int) (cmRcBuf0[2]<<8)+cmRcBuf0[3];
	        cmTrBuf0[1]=0x03;//����� �������
	        //���������� ���� ������ � ������(��������� �� 2 ��� ��� �������� 16-����.)
	        cmTrBuf0[2]=(unsigned char) 2*kolreg;

	        for(i_for=0; i_for<kolreg; i_for++)
	        {
	          cmTrBuf0[3+(2*i_for)]=Hi_short(reg_data[(first_reg+i_for)]);
	          cmTrBuf0[(3+(2*i_for)+1)]=Low_short(reg_data[(first_reg+i_for)]);

	        }

	        //��������� ��
	        tempCRC=GetCRC16(cmTrBuf0,((unsigned char)2*kolreg+3));
	        cmTrBuf0[2*kolreg+3]=Low(tempCRC);
	    	cmTrBuf0[2*kolreg+4]=Hi(tempCRC);
	      	return (cmTrBuf0[2]+5);

               }

  case 0x06:{//������ ������ ��������,�������� ��������� ��������� �������
	        //����� ������������ ��������
	        first_reg=(int) (cmRcBuf0[2]<<8) + cmRcBuf0[3];
	     	reg_data[first_reg]=(cmRcBuf0[4]<<8)+cmRcBuf0[5];

	     	for(i_for=1; i_for<=7; i_for++)
	        {cmTrBuf0[i_for]=cmRcBuf0[i_for];}
	     	    return 8;
	        }

  case 0x10:{//������� 16: 3����� ���������� ���������
	        //����� ������� ������������ ��������
	        first_reg=(int) (cmRcBuf0[2]<<8) + cmRcBuf0[3];
	        //���-�� ���������
	        kolreg=(int) (cmRcBuf0[4]<<8) + cmRcBuf0[5];
	        // ���-�� ���� � ���� ������
	        TempI=(int) cmRcBuf0[6];

	        for(i_for=0; i_for<TempI; i_for=i_for+2)
	        {
	         reg_data[first_reg+cnt_reg16]=(cmRcBuf0[7+i_for]<<8)+cmRcBuf0[7+(i_for+1)];
	         cnt_reg16++;
	        }

	        cnt_reg16=0;

	        for(i_for=1; i_for<=5; i_for++)
	        {
	        cmTrBuf0[i_for]=cmRcBuf0[i_for];
	        }

	        tempCRC=GetCRC16(cmTrBuf0,6);
	        cmTrBuf0[6]=Low(tempCRC);
	        cmTrBuf0[7]=Hi(tempCRC);

	       // refresh_data=TRUE;//������ ��� ���������� ������


	        return 8;

	     }

          default:
				   {
				   return
				   ErrorMessage(0x01);//������������ �������
				   }//end default
     }//end switch
}//end ModBus





 void StartTrans0(void)
 {
         TrCount=0;

         mb_receive_disable;//��������� ���������� �� ������
         mb_trans_enable;// ��������� ���������� �� ��������


 	  	//set_led6;//�������� �E15
 	  	USART_SendData(USART1,cmTrBuf0[TrCount]);
 	  	TrCount++;

 }

//���������� USART3
//���������� USART3
void USART1_IRQHandler(void)
{
	//���������� �� ������
	//GPIO_SetBits(LED2_PORT, LED2);
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���������� �� ������?
   {
  		clr_int_rx;//������� �������� ����������
  		 //set_led7;//�������� ��14

			if(StartRec==FALSE)//���� StartRec=0 ��� ������ ����-�������� �����
			{
				StartRec=TRUE;
				RcCount=0;
				cmRcBuf0[RcCount++]=USART_ReceiveData(USART1);//���������� � ������  �������� ������
				enable_tim_mb;//��������� ������ �������
				restart_tim_mb;//������������� ������
			}
  		// ����� ���������� �����
			else
			{
				if(RcCount<MaxLenRecBuf)//���� ����� ��� �� ����������
				{
					cmRcBuf0[RcCount++]=USART_ReceiveData (USART1);//���������� � ������  �������� ������
				}
				else//����� ����������
				{
					cmRcBuf0[MaxLenRecBuf-1]=USART_ReceiveData (USART1);//���������� � ������  �������� ������
				}
				restart_tim_mb;//������������� ������
			}
  	 }

  	//���������� �� ���������� ��������
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)//���������� �� ���������� ��������?
  	{
    	clr_int_txend;//������� �������� ����������
  		  if(TrCount<cNumTrByte0)//���� ������� ������������ ���� ������ ���-�� ������������ ����
  		{
  			USART_SendData(USART1,cmTrBuf0[TrCount]);//�������� �� ������� cmTrBuf0 ������ � USART
  			TrCount++;                               //���� �� ��������� ��� ������
  		}
  		 else
  		{
  			TrCount=0;//�������� ������� ������������ ����

  		 //reset_led7;
  			mb_receive_enable;//��������� ���������� �� ������
  			mb_trans_disable;//��������� ���������� �� ��������
  			//GPIO_ResetBits(LED2_PORT, LED2);

  		}
  	}
}


    //���������� �� �������
    //***************************************************************************
    void TIM3_IRQHandler(void)
    {
    	clr_int_tim_mb;//������� �������� ����������

    	if(StartRec==TRUE)//���� StartRec=1 �� ������ ���� ������ � ��������� ���������� �� ��������� �������
    	   {
    		StartRec=FALSE;//�������� StartRec ��� ���������� ������� ���������� �� ������ ������
    		cNumRcByte0=RcCount;//
    		bModBus=TRUE;//����� ������������ �������
    		disable_tim_mb;//��������� ������ �������
    	   }
    }

 void do_modbus(void)
 {
	 if(bModBus==TRUE){
	 cNumTrByte0=ModBus(cNumRcByte0); //�������� ������������ ��������� ��������� ��������� ModBus.
	 											 //� ������������ ���������� ���������� cNumRcByte0(���-�� �������� ����).
	 											 //����� ���������� ������������ � cNumTrByte0 ������������ ��������
	 											 // ���������� ������������ ����(�������� ��������� ModBus)
	 if(cNumTrByte0!=0)

	 StartTrans0();
	 bModBus=FALSE;

 }
 }

