//###########################################################################
//
// FILE:	systemExternalInterrupt.c
//
// TITLE:	DSP281x system External 1ms interrupt.
//
//Ŀ�ģ�2812ϵͳ�ⲿ1ms��ʱ�ж����
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Added support for C++
//###########################################################################

#include "systemExternalInterrupt.h"
#include "systemDefine.h"
#include "dualram.h"
#include "DSP281x_Device.h"
#include "System.h"


//#include "systemMcbsp.h"
//#include "systemDebug.h"
//#include "systemAd.h"
//#include "systemTest.h"
//#include "systemTimer.h"
//�ⲿ1ms��ʱ�жϳ�ʼ��   XINT1 CPU INT1  XINT1 in the PIE: Group 1 interrupt 4
//�ϵ�Ĭ��0 Interrupt generated on a falling edge (high-to-low transition)


extern unsigned int dat[LENGTH]; 		//���������
extern unsigned int buf[LENGTH+5]; 		//����������
extern unsigned int Parity;//��־λ����Ϊ1��д������������Ϊ0��д��ż����
extern unsigned int flag;			//�����ñ�־������ֹͣ��������,0ͣ1��

unsigned int INT2FLAG=0;//��־λ�������ε�INT2��Ϊ1��ż������Ϊ0

void systemExternalInterruptInit(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers
   	PieVectTable.XINT1 = &system_external_interrupt0_isr;//ָ���ⲿ1ms�жϷ����ӳ���
	

	PieVectTable.XINT2 = &system_external_interrupt1_isr;//ָ��xint2 �ⲿ1ms�жϷ����ӳ���
   	EDIS;    // This is needed to disable write to EALLOW protected registers

}

//xint1 ��Ӧ�ⲿ1ms�жϣ����ڸ���������IO�ڷ�ת
interrupt void system_external_interrupt0_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
/* #if EXTERNAL_INTERRUPT_SELECT == 0 */
 	systemExternalInterruptHandle();
/* 	                                  */
/* #endif                             */
}


//xint2 ��Ӧ��fpga�в������жϣ�����ָ���ȡ
interrupt void system_external_interrupt1_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
 	systemExternalInterrupt2Handle();  
}



//F2812���ⲿ1ms��ʱ�жϴ��������������������е���
//xint1 ��Ӧ�ⲿ1ms�жϣ����ڶ�ȡ���������㷨�Ƿ�����
void systemExternalInterruptHandle(void)
{
	unsigned int i;                                                                             
	for (i=0;i<LENGTH;i++)
	{
		if (dat[i]==65535)
			dat[i]=0;
		else
			dat[i]++;
	}
			
	//���õ������м����д��RAM��
	if (Parity)
	//�����д��������
	{
		B_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x1);
		Parity=0;
	}
	//�����д��ż����
	else
	{
		B_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x101);
		Parity=1;
	}
	
}


//xint2 ��Ӧ��fpga�в������жϣ�����ָ���ȡ
void systemExternalInterrupt2Handle(void)
{
	//unsigned int i;
	INT2FLAG=!INT2FLAG;
	if (INT2FLAG) //������
	{
		flag=1;
		//if ((dat[0]==0) || (dat[0]==9))
		//����Ʊ�ʾ�����ж���
//			LED_DAT |= 0x0800;
//		else
//			LED_DAT &= ~0x0800;
	}
	else //ż����
	{
		flag=0;
		//LED_DAT &= ~0x0800;
	}
}

//�������������FPGA�жϣ���xint2�Ͳ���ʹ����
  

