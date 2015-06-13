/******************************************************************/
/*-------------------20150502-LONGREJOY---------------------------*/        
/******************************************************************/
//A_GPIO_CHECK  62  C5TRIP#/GPIOB14
//A_GPIOA11 ����������LED��Ϊ���Խ�����
/*****************ͷ�ļ�********************/  
#include "DSP281x_Device.h"
#include "System.h"

#include "systemCrc16.h"
#include "systemInterrupt.h"
#include "systemExternalInterrupt.h"
#include "dualram.h"

#include "systemDefine.h"

/*��Ϊ���������ⲿ��1MS�жϽ���IO�ڷ�ת�Ӷ��õ�500Hz״̬���壬�ʲ��ö�ʱ��0���м�ʱҲ����*/



/***************ȫ�ֱ�������****************/  
unsigned int buf[LENGTH+5]; 	//����������
unsigned int dat[LENGTH]; 		//���������
unsigned int Parity=1;			//д����ż�α�־λ
unsigned int flag=0;			//�����ñ�־������ֹͣ/�����������ݣ���1��ʼ��0��ֹͣ
//unsigned int flag=1;			//������


/****************��������*******************/ 
void Init_LED(void);
//void Init_Timer0(void);
//interrupt void Timer0_INT(void); 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:������			                */
/*------------------------------------------*/ 
void main(void)
{
	//int 16b  long 32b
	//unsigned int i,odd_des,even_des;
	//unsigned long temp;
	//unsigned int FirSt;	//��־λ����Ϊ��һ���ϵ���Ϊ1����Ϊ������λ��Ϊ0
	unsigned int Readflag=0; //��ȡ��־λ����ȡ����1���Ժ��ٶ�ȡ
	unsigned int i,j;

	//��ʼ��
	InitSysCtrl();	 	//ϵͳ��ʼ���ӳ�����DSP28_sysctrl.c�У��������ж�
	Init_LED();	 		//��ʼ��״̬LED		A11
	Init_XINTF();	 	//��ʼ��XINTF��Zone6
	systemExternalInterruptInit();
	systemInterruptEnable();	//��ʼ���ⲿ�жϣ��������жϣ�//l.r.20150505���ж���䱻ע����
	//�ȹر�INT1���ȴ�INT2���ź�֮���ٿ�
	XIntruptRegs.XINT1CR.all=0x0;//�رչܽ�xint1���жϣ�Ĭ�����½��ش����жϣ���Ϊ�����أ���0x5

	EINT;   // Enable Global interrupt INTM				//l.r.20150505��ʱ���ж�
    ERTM;   // Enable Global realtime interrupt DBGM		//l.r.20150505��ʱ���ж�
	//����ƴ��������������
	LED_DAT &= ~0x0C00;
	while(1)
	{	
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		//��1s
		for (i=0;i<1000;i++)
			for (j=0;j<4;j++)
				DELAY_US(250);
		//��1s
		LED_DAT |= 0x0800;
		for (i=0;i<1000;i++)
			for (j=0;j<4;j++)
				DELAY_US(250);
		//��
		LED_DAT &= ~0x0800;
	}

} 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��״̬LED�˿�Ϊ���������A8  */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
	LED_DAT |= 0x0800;	//��������Ϊ��ʼ��
}   



 
