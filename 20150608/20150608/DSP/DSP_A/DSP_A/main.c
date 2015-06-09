/******************************************************************/
/*-------------------20150426-LONGREJOY---------------------------*/        
/******************************************************************/
//A_GPIO_CHECK  62  C5TRIP#/GPIOB14
//A_GPIOA11 ����������LED��Ϊ���Խ�����
//A_GPIOA10 ������FPGA���͸�λ�ɹ��ź�
/*****************ͷ�ļ�********************/  
#include "DSP281x_Device.h"
#include "System.h"

#include "systemCrc16.h"
#include "systemInterrupt.h"
#include "systemExternalInterrupt.h"
#include "dualram.h"

#include "systemDefine.h"



/*��Ϊ���������ⲿ��1MS�жϽ���IO�ڷ�ת�Ӷ��õ�500Hz״̬���壬�ʲ��ö�ʱ��0���м�ʱҲ����*/

/****************�˿ں궨��*****************/
#define StaPul_DIR GpioMuxRegs.GPBDIR.all		//״̬���巢���ڷ���Ĵ���
#define StaPul_DAT GpioDataRegs.GPBDAT.all		//״̬���巢�������ݼĴ���

#define LED_DIR GpioMuxRegs.GPADIR.all		//״̬���巢���ڷ���Ĵ���
#define LED_DAT GpioDataRegs.GPADAT.all		//״̬���巢�������ݼĴ���

#define Check_DIR GpioMuxRegs.GPADIR.all		//�źŷ����ڷ���Ĵ���
#define Check_DAT GpioDataRegs.GPADAT.all		//�źŷ��������ݼĴ���

/***************ȫ�ֱ�������****************/  
unsigned int buf[LENGTH+5]; 	//����������
unsigned int dat[LENGTH]; 		//���������
unsigned int Parity=1;			//д����ż�α�־λ
unsigned int flag=0;			//�����ñ�־������ֹͣ��������


/****************��������*******************/ 
void Init_StaPul(void);
void Init_LED(void);
void Init_Check(void);


/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:������			                */
/*------------------------------------------*/ 
void main(void)
{
	unsigned int i,odd_des,even_des;
	unsigned int FirSt;	//��־λ����Ϊ��һ���ϵ���Ϊ1����Ϊ������λ��Ϊ0

	//��ʼ��
	InitSysCtrl();	 //ϵͳ��ʼ���ӳ�����DSP28_sysctrl.c�У��������ж�
	Init_StaPul();	 //��ʼ��״̬����
	Init_LED();
	Init_Check();
	Init_XINTF();	 //��ʼ��XINTF��Zone6
	systemExternalInterruptInit();
	systemInterruptEnable();	//��ʼ���ⲿ�жϣ��������ж�	//l.r.20150505���ж���䱻ע����
	
	for (i=0;i<LENGTH;i++)
		dat[i]=0;

	//�ȶ�������RAM�ж�ȡDESλ�ж��Ƿ��ǵ�һ���ϵ�
	//������oddд��0x100000��ʼ��LENGTH���ռ�
	//ż����evenд��0x140000��ʼ��LENGTH���ռ�
	odd_des=Read_RAM(0x3);//20150515 l.r. ��Ϊ��0x1λ�ÿ�ʼд���һ������
	even_des=Read_RAM(0x103);//20150515 l.r. ��Ϊ��0x40001λ�ÿ�ʼд���һ������
	if ((odd_des==0) && (even_des==0))
		FirSt=1;
	else
		FirSt=0;
	//�˴��ж�����Ҳ�����Ƿ�0��1����Ҫ���ϵ�FPGA��IP��������ʲô����0����65535
	
	//��λ�ϵ�Ĵ����߼�
	if (FirSt==0)
	{
		//�����������
		if (Unpack(0x1)==OK)		//�������������ȷ
		{
			for (i=0;i<LENGTH;i++)
				dat[i]=buf[i+4];	//RAM�������Ѿ���buff�У������е����ݶ���ȡ��dat�У���Ϊ�м����
			//���������㷨
			//���õ������м����д��RAM��
			//��FPGA���͸�λ�ɹ����ź�
			Check_DAT &= ~0x0400;
		}
		//���ż������
		else if (Unpack(0x101)==OK)	//���ż��������ȷ
		{
			for (i=0;i<LENGTH;i++)
				dat[i]=buf[i+4];	//RAM�������Ѿ���buff�У������е����ݶ���ȡ��dat�У���Ϊ�м����
			//���������㷨
			//���õ������м����д��RAM��
			//��FPGA���͸�λ�ɹ����ź�
			Check_DAT &= ~0x0400;
		}

	}
	//��һ���ϵ�Ĵ����߼����븴λ�ϵ���RAM����ȡ���м����ݺ����һ�£�����û��else��֧
	//else 
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	DELAY_US(250);
	EINT;   // Enable Global interrupt INTM           	//l.r. 20150505�ڴ˴���
    ERTM;   // Enable Global realtime interrupt DBGM	//l.r. 20150505�ڴ˴���
	//����ƴ��������������
	LED_DAT &= ~0x0800;

	while(1)
	{
		//�����㷨---�˴���Ϊ�㷨�򵥷ŵ�1ms�ж���ȥ��
		if (flag==1)
			XIntruptRegs.XINT1CR.all=0x0;//�رչܽ�xint1���ж�
	}

} 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��״̬����˿�Ϊ���������   */
/*------------------------------------------*/ 
void Init_StaPul(void)
{
	EALLOW;
	StaPul_DIR |= 0x4000;
	EDIS;
	StaPul_DAT &= ~0x4000;	//������״̬�������Ϊ��ʼ��
}  

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ����FPGA�����źŵĶ˿�A10    */
/*------------------------------------------*/ 
void Init_Check(void)
{
	EALLOW;
	Check_DIR |= 0x0400;
	EDIS;
	Check_DAT |= 0x0400;	//��������Ϊ��ʼ��
}   
 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��״̬LED�˿�Ϊ���������A11 */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
	LED_DAT |= 0x0800;	//��������Ϊ��ʼ��
}   



 
