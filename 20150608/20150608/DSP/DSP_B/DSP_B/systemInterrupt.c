//###########################################################################
//
// FILE:	systemInterrupt.h
//
// TITLE:	DSP281x system interrupt init and handle.
//
//Ŀ�ģ���ϵͳ����ϵͳ�ж�
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Added support for C++
//###########################################################################

#include "systemInterrupt.h"
//#include "systemDefine.h"

//ϵͳ�ж�������ӳ��ͳ�ʼ��
//ÿ��������жϳ�ʼ������ÿ���ֱ��ģ����ʵ�֡�
/* void systemInterruptInit(void)            */
/* {                                         */
/* 	DINT;//���ж�                            */
/* 	InitPieCtrl();//��ʼ��pie�Ĵ���          */
/* 	IER = 0x0000;//��ֹ���е��ж�            */
/*    	IFR = 0x0000;                        */
/* 	InitPieVectTable();//��ʼ��pie�ж������� */
/*                                           */
/* }                                         */


void systemInterruptEnable(void)
{
	PieCtrlRegs.PIECRTL.bit.ENPIE=1;//ENABLE THE PIE BLOCK
	//***********************ʹ�ܶ�ʱ��0��Ӧ���ж�  GROUP7 interrupt 7 CPU INT1
	
/*    if(sSystemPar.cHostFlag == SLAVE)//�ӻ�����Ҫ���ö�ʱ��0 */
/* 	{                                                          */
/* 	// Enable CPU INT1 which is connected to CPU-Timer 0:      */
/*    	IER |= M_INT1;                                         */
/* 	// Enable TINT0 in the PIE: Group 1 interrupt 7            */
/*    	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;                     */
/* 	}	                                                       */
   
   // Enable global Interrupts and higher priority real-time debug events:
   
   
   
   //***********************ʹ���ⲿ�ж�1����Ӧ1ms�ⲿ�ź��ж�XINT1 CPU INT1  XINT1 in the PIE: Group 1 interrupt 4
   	// Enable CPU INT1 which is connected to XINT0:
   IER |= M_INT1;
	// Enable XINT0 in the PIE: Group 1 interrupt 4
//#if EXTERNAL_INTERRUPT_SELECT == 0   //���ⲿ�ж�1--���ⲿ1ms��ʱ�ź�����
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
	XIntruptRegs.XINT1CR.all=0x0;//�򿪹ܽ�xint1���жϣ�Ĭ�����½��ش����жϣ���Ϊ�����أ���0x5
//#endif
//#if EXTERNAL_INTERRUPT_SELECT == 1  //���ⲿ�ж�2--��fpga���������1ms��ʱ�ж�����
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;//xint2
	XIntruptRegs.XINT2CR.all=0x1;//�򿪹ܽ�xint2���жϣ�Ĭ�����½��ش����жϣ���Ϊ�����أ���0x5
//#endif

	//********************************ȫ���ж�ʹ��
   //EINT;   // Enable Global interrupt INTM				//l.r.20150505�Ȳ����ж�
   //ERTM;   // Enable Global realtime interrupt DBGM		//l.r.20150505�Ȳ����ж�
}
