//###########################################################################
//
// FILE:	systemExternalInterrupt.h
//
// TITLE:	DSP281x system External 1ms interrupt.
//
//目的：2812系统外部1ms定时中断相关
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

//外部中断1初始化，对应1ms外部中断,下降沿触发
void systemExternalInterruptInit(void);

//外部中断1中断处理函数 1ms 
interrupt void system_external_interrupt0_isr(void);


//xint2 对应中断，调试阶段，用fpga产生1ms的定时信号
interrupt void system_external_interrupt1_isr(void); 

//F2812在外部1ms定时中断处理函数里面对这个函数进行调用
void systemExternalInterruptHandle(void);
void systemExternalInterrupt2Handle(void);

void localGpioClkOut(void);

#endif
