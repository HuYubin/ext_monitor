/**************************************************************************
  
                     THE REAL TIMER DS1302 DRIVER LIB
  
               COPYRIGHT (c)   2005 BY JJJ.
                         --  ALL RIGHTS RESERVED  --
  
   File Name:       DS1302.h
   Author:          Jiang Jian Jun
   Created:         2003/7/21
   Modified:  NO
   Revision:   1.0
  
***************************************************************************/
#include "stm32f10x.h"
#include "ds1302.h"

GPIO_InitTypeDef GPIO_InitStructure;

//ʱ��������
#define DS1302_SET_CLK GPIO_SetBits(GPIOC, GPIO_Pin_10)               
#define DS1302_CLR_CLK GPIO_ResetBits(GPIOC, GPIO_Pin_10)

//����������
#define DS1302_SET_IO GPIO_SetBits(GPIOC, GPIO_Pin_11)                
#define DS1302_CLR_IO GPIO_ResetBits(GPIOC, GPIO_Pin_11)
#define DS1302_READ_IO GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) 
#define DS1302_IO_IN    {\
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  \
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  \
        GPIO_Init(GPIOC, &GPIO_InitStructure); }
#define DS1302_IO_OUT   {\
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  \
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  \
        GPIO_Init(GPIOC, &GPIO_InitStructure); }

//��λ������          
#define DS1302_SET_RST GPIO_SetBits(GPIOC, GPIO_Pin_12)                
#define DS1302_CLR_RST GPIO_ResetBits(GPIOC, GPIO_Pin_12)


#define AM(X) X
#define PM(X) (X+12)               // ת��24Сʱ��
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR  0x84 
#define DS1302_WEEK  0x8A
#define DS1302_DAY  0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR  0x8C
#define DS1302_RAM(X) (0xC0+(X)*2)    //���ڼ��� DS1302_RAM ��ַ�ĺ�

void DS1302InputByte(unsigned char d)  //ʵʱʱ��д��һ�ֽ�(�ڲ�����)
{ 
    unsigned char i;

    for(i = 0; i < 8; i++)
    {
        if((d>>i)&0x1){
          DS1302_SET_IO;
        }else{
          DS1302_CLR_IO;
        }
        DS1302_SET_CLK;
        DS1302_CLR_CLK;
    } 
}

unsigned char DS1302OutputByte(void)  //ʵʱʱ�Ӷ�ȡһ�ֽ�(�ڲ�����)
{ 
    unsigned char i = 0;
    unsigned char d = 0;
    unsigned char j = 0;
    
    for(i = 0; i < 8; i++)
    {
        j= DS1302_READ_IO;
        d |= (j<<i);
        DS1302_SET_CLK;
        DS1302_CLR_CLK;
    } 
    return(d); 
}

//ucAddr: DS1302��ַ, ucData: Ҫд������
void Write1302(unsigned char ucAddr, unsigned char ucDa) 
{
    DS1302_CLR_RST;
    DS1302_CLR_CLK;
    DS1302_SET_RST;
    DS1302_IO_OUT;
    DS1302InputByte(ucAddr);        // ��ַ������ 
    DS1302InputByte(ucDa);        // д1Byte����
    DS1302_SET_CLK;
    DS1302_CLR_RST;
}

unsigned char Read1302(unsigned char ucAddr) //��ȡDS1302ĳ��ַ������
{
    unsigned char ucData;
    DS1302_CLR_RST;
    DS1302_CLR_CLK;
    DS1302_SET_RST;
    DS1302_IO_OUT;
    DS1302InputByte(ucAddr|0x01);        // ��ַ������
    DS1302_IO_IN; 
    ucData = DS1302OutputByte();         // ��1Byte����
    DS1302_SET_CLK;
    DS1302_CLR_RST;
    return(ucData);
}

//�Ƿ�д����
void DS1302_SetProtect(unsigned char flag)        
{
 if(flag)
  Write1302(0x8E,0x10);
 else
  Write1302(0x8E,0x00);
}

void DS1302_GetTime(unsigned char *Time)
{
 Time[5] = Read1302(DS1302_SECOND);
 Time[4] = Read1302(DS1302_MINUTE);
 Time[3] = Read1302(DS1302_HOUR);
 Time[2] = Read1302(DS1302_DAY);
 Time[1] = Read1302(DS1302_MONTH);
 Time[0] = Read1302(DS1302_YEAR); 
}
void DS1302_SetTime(unsigned char *Time)
{

	DS1302_SetProtect(0); // ��ֹд����
	Write1302(DS1302_SECOND,Time[5]);
	Write1302(DS1302_MINUTE,Time[4]);
	Write1302(DS1302_HOUR,Time[3]);
	Write1302(DS1302_DAY,Time[2]);
	Write1302(DS1302_MONTH,Time[1]);
	Write1302(DS1302_YEAR,Time[0]);
	DS1302_SetProtect(1); 
}
void Initial_DS1302(void)
{
	// DS1302 ���ų�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* Configure PC10��PC11 and PC12 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	
	
}

//��DS1302д��ʱ������(���ֽڷ�ʽ)
void BurstWrite1302(unsigned char *pWClock) 
{
    unsigned char i;
    Write1302(0x8e,0x00);          // ��������,WP=0,д����?
    DS1302_CLR_RST;
    DS1302_CLR_CLK;
    DS1302_SET_RST;
    DS1302_IO_OUT;
    DS1302InputByte(0xbe);         // 0xbe:ʱ�Ӷ��ֽ�д����
    for (i = 8; i>0; i--)       //8Byte = 7Byte ʱ������ + 1Byte ����
    {
        DS1302InputByte(*pWClock);  // д1Byte����
        pWClock++;
    }
    DS1302_SET_CLK;
    DS1302_CLR_RST;
}

//��ȡDS1302ʱ������(ʱ�Ӷ��ֽڷ�ʽ)
void BurstRead1302(unsigned char *pRClock) 
{
    unsigned char i;
    DS1302_CLR_RST;
    DS1302_CLR_CLK;
    DS1302_SET_RST;
    DS1302_IO_OUT;
    DS1302InputByte(0xbf);              // 0xbf:ʱ�Ӷ��ֽڶ����� 
    for (i=8; i>0; i--) 
    {
       DS1302_IO_IN;
       *pRClock = DS1302OutputByte();   // ��1Byte���� 
       pRClock++;
    }
    DS1302_SET_CLK;
    DS1302_CLR_RST;
}

// �Ƿ�ʱ��ֹͣ
void DS1302_TimeStop(unsigned char flag)           
{
 unsigned char Data;
 Data=Read1302(DS1302_SECOND);
 DS1302_SetProtect(0);
 if(flag)
  Write1302(DS1302_SECOND, Data|0x80);
 else
  Write1302(DS1302_SECOND, Data&0x7F);
}
