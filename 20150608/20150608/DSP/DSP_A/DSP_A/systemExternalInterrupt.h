//###########################################################################
//
// FILE:	systemExternalInterrupt.h
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
#ifndef _SYSTEMEXTERNALINTERRUPT_H
#define _SYSTEMEXTERNALINTERRUPT_H
#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
//#include "DSP281x_Examples.h"   // DSP281x Examples Include File

//�ⲿ�ж�1��ʼ������Ӧ1ms�ⲿ�ж�,�½��ش���
void systemExternalInterruptInit(void);

//�ⲿ�ж�1�жϴ����� 1ms 
interrupt void system_external_interrupt0_isr(void);


//xint2 ��Ӧ�жϣ����Խ׶Σ���fpga����1ms�Ķ�ʱ�ź�
interrupt void system_external_interrupt1_isr(void); 

//F2812���ⲿ1ms��ʱ�жϴ��������������������е���
void systemExternalInterruptHandle(void);
void systemExternalInterrupt2Handle(void);

void localGpioClkOut(void);

#endif
