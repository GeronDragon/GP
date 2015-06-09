/******************************************************************/
/*Copyright (C), 2008-2009, ������ӣ�LiTian Tech.Co.Ltd.		  */
/*  Module Name   : Timer0        							  	  */
/*  File Name     : main.c	   									  */        
/*  Author   	  : ���                                        */     
/*  Create Date   : 2009/12/27                                    */      
/*  Version   	  : 2.0                                           */          
/*  Function      : 			         					      */                       
/*  Description   : 								              */            
/*  Support       : www.LT430.com 				                  */             
/******************************************************************/


//GPIOA11
/*****************ͷ�ļ�********************/  
#include "DSP281x_Device.h"
#include "System.h"

/****************�˿ں궨��*****************/
#define LED_DIR GpioMuxRegs.GPADIR.all
#define LED_DAT GpioDataRegs.GPADAT.all

/****************�����궨��*****************/ 

/***************ȫ�ֱ�������****************/
unsigned int temp;	//ָʾ��������ܵ���ʾ״̬ 

/****************��������*******************/ 
void Init_LED(void);
void Init_Timer0(void);
interrupt void Timer0_INT(void); 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:������			                */
/*------------------------------------------*/ 
void main(void)
{
	InitSysCtrl();  // ϵͳ��ʼ���ӳ�����DSP28_sysctrl.c��
	Init_LED(); //���÷�������ܶ˿����
	Init_Timer0();
	//temp=0x0001;
	while(1);   
} 

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ����������ܶ˿�Ϊ���       */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
}   

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��Timer0				        */
/*------------------------------------------*/ 
void Init_Timer0(void)
{
    CpuTimer0Regs.PRD.all  = 120000;//���ö�ʱ������
	//CpuTimer0Regs.PRD.all  = 119999;//���ö�ʱ������
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;
    CpuTimer0Regs.TCR.bit.TSS = 1;//ֹͣ��ʱ��
	CpuTimer0Regs.TCR.bit.TIE = 1;//ʹ�ܶ�ʱ��0�ж�
	DINT;   // �ر����ж�
	EALLOW;	// ����Ĵ�������
	PieVectTable.TINT0 = &Timer0_INT;//��Timer0�жϺ�����ڸ���PIE������
	EDIS;   // ʹ�ܼĴ�������
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;// ʹ��PIE�ڵ�Timer0�ж� 
    IER |= M_INT1;// ʹ�� CPU INT 1	    
   	EINT;   // ȫ���ж�ʹ��INTM
   	CpuTimer0Regs.TCR.bit.TRB = 1;//��ʱ����װ
    CpuTimer0Regs.TCR.bit.TSS = 0;//������ʱ�� 
}  

/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:Timer0�жϴ�����				*/
/*------------------------------------------*/  
interrupt void Timer0_INT(void)
{
 	DINT;//�ر����ж�
	CpuTimer0Regs.TCR.bit.TSS = 1;//ֹͣ��ʱ��
	CpuTimer0Regs.TCR.bit.TIF = 1;//�����жϱ�־λ
   	//if(temp==0x0100)
	//	temp=0x0001;
	LED_DAT=(LED_DAT^0x0800);
	//temp<<=1;
    PieCtrlRegs.PIEACK.all = 0x0001;   //��Ӧ�ж�
	CpuTimer0Regs.TCR.bit.TRB = 1;//��ʱ����װ
    CpuTimer0Regs.TCR.bit.TSS = 0;//������ʱ��
	EINT;   //�����ж�
} 

 
