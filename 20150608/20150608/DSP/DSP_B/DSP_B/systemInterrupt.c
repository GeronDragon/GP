//###########################################################################
//
// FILE:	systemInterrupt.h
//
// TITLE:	DSP281x system interrupt init and handle.
//
//目的：对系统配置系统中断
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

//系统中断向量表映射和初始化
//每个外设的中断初始化，在每个分别的模块里实现。
/* void systemInterruptInit(void)            */
/* {                                         */
/* 	DINT;//关中断                            */
/* 	InitPieCtrl();//初始化pie寄存器          */
/* 	IER = 0x0000;//禁止所有的中断            */
/*    	IFR = 0x0000;                        */
/* 	InitPieVectTable();//初始化pie中断向量表 */
/*                                           */
/* }                                         */


void systemInterruptEnable(void)
{
	PieCtrlRegs.PIECRTL.bit.ENPIE=1;//ENABLE THE PIE BLOCK
	//***********************使能定时器0对应的中断  GROUP7 interrupt 7 CPU INT1
	
/*    if(sSystemPar.cHostFlag == SLAVE)//从机才需要配置定时器0 */
/* 	{                                                          */
/* 	// Enable CPU INT1 which is connected to CPU-Timer 0:      */
/*    	IER |= M_INT1;                                         */
/* 	// Enable TINT0 in the PIE: Group 1 interrupt 7            */
/*    	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;                     */
/* 	}	                                                       */
   
   // Enable global Interrupts and higher priority real-time debug events:
   
   
   
   //***********************使能外部中断1，对应1ms外部信号中断XINT1 CPU INT1  XINT1 in the PIE: Group 1 interrupt 4
   	// Enable CPU INT1 which is connected to XINT0:
   IER |= M_INT1;
	// Enable XINT0 in the PIE: Group 1 interrupt 4
//#if EXTERNAL_INTERRUPT_SELECT == 0   //打开外部中断1--由外部1ms定时信号驱动
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
	XIntruptRegs.XINT1CR.all=0x0;//打开管脚xint1外中断，默认是下降沿触发中断，如为上升沿，则0x5
//#endif
//#if EXTERNAL_INTERRUPT_SELECT == 1  //打开外部中断2--由fpga自身产生的1ms定时中断驱动
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;//xint2
	XIntruptRegs.XINT2CR.all=0x1;//打开管脚xint2外中断，默认是下降沿触发中断，如为上升沿，则0x5
//#endif

	//********************************全局中断使能
   //EINT;   // Enable Global interrupt INTM				//l.r.20150505先不开中断
   //ERTM;   // Enable Global realtime interrupt DBGM		//l.r.20150505先不开中断
}
