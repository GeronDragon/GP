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
//#include "systemDefine.h"
//#include "systemMcbsp.h"
//#include "systemDebug.h"
//#include "systemAd.h"
//#include "systemTest.h"
//#include "systemTimer.h"
//�ⲿ1ms��ʱ�жϳ�ʼ��   XINT1 CPU INT1  XINT1 in the PIE: Group 1 interrupt 4
//�ϵ�Ĭ��0 Interrupt generated on a falling edge (high-to-low transition)
//#define LENGTH 10	//��Ҫ���ݵ��м���������������ȣ� һ��Ҫ��main.c�е�define��Ӧ�ϣ�����������������������������������������������
extern unsigned int dat[LENGTH]; 		//���������
extern unsigned int buf[LENGTH+5]; 		//����������
extern unsigned int Parity;//��־λ����Ϊ1��д������������Ϊ0��д��ż����
extern unsigned int flag;

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

void localGpioClkOut(void)
{
	GpioDataRegs.GPBTOGGLE.bit.GPIOB14 = 1;//check gpio Flip
}

//F2812���ⲿ1ms��ʱ�жϴ��������������������е���
//xint1 ��Ӧ�ⲿ1ms�жϣ����ڸ���������IO�ڷ�ת
void systemExternalInterruptHandle(void)
{
	unsigned int i;
	localGpioClkOut();
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
		A_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x1);
		Parity=0;
	}
	//�����д��ż����
	else
	{
		A_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x101);
		Parity=1;
	}
	
	if (dat[0]==10)
		flag=1;	
}


//xint2 ��Ӧ��fpga�в������жϣ�����ָ���ȡ
void systemExternalInterrupt2Handle(void)
{
	;
}

//�������������FPGA�жϣ���xint2�Ͳ���ʹ����
  

