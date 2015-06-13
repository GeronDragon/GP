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
#ifndef _SYSTEMINTERRUPT_H
#define _SYSTEMINTERRUPT_H
#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
//#include "DSP281x_Examples.h"   // DSP281x Examples Include File

extern void systemInterruptInit(void);

extern void systemInterruptEnable(void);
#endif
