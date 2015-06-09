/******************************************************************/
/*Copyright (C), 2008-2009, 力天电子，LiTian Tech.Co.Ltd.		  */
/*  Module Name   : Timer0        							  	  */
/*  File Name     : main.c	   									  */        
/*  Author   	  : 侯长波                                        */     
/*  Create Date   : 2009/12/27                                    */      
/*  Version   	  : 2.0                                           */          
/*  Function      : 			         					      */                       
/*  Description   : 								              */            
/*  Support       : www.LT430.com 				                  */             
/******************************************************************/


//GPIOA11
/*****************头文件********************/  
#include "DSP281x_Device.h"
#include "System.h"

/****************端口宏定义*****************/
#define LED_DIR GpioMuxRegs.GPADIR.all
#define LED_DAT GpioDataRegs.GPADAT.all

/****************常量宏定义*****************/ 

/***************全局变量定义****************/
unsigned int temp;	//指示发光二极管的显示状态 

/****************函数声明*******************/ 
void Init_LED(void);
void Init_Timer0(void);
interrupt void Timer0_INT(void); 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:主函数			                */
/*------------------------------------------*/ 
void main(void)
{
	InitSysCtrl();  // 系统初始化子程序，在DSP28_sysctrl.c中
	Init_LED(); //设置发光二极管端口输出
	Init_Timer0();
	//temp=0x0001;
	while(1);   
} 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化发光二极管端口为输出       */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
}   

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化Timer0				        */
/*------------------------------------------*/ 
void Init_Timer0(void)
{
    CpuTimer0Regs.PRD.all  = 120000;//设置定时器周期
	//CpuTimer0Regs.PRD.all  = 119999;//设置定时器周期
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;
    CpuTimer0Regs.TCR.bit.TSS = 1;//停止定时器
	CpuTimer0Regs.TCR.bit.TIE = 1;//使能定时器0中断
	DINT;   // 关闭总中断
	EALLOW;	// 解除寄存器保护
	PieVectTable.TINT0 = &Timer0_INT;//用Timer0中断函数入口更新PIE向量表
	EDIS;   // 使能寄存器保护
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;// 使能PIE内的Timer0中断 
    IER |= M_INT1;// 使能 CPU INT 1	    
   	EINT;   // 全局中断使能INTM
   	CpuTimer0Regs.TCR.bit.TRB = 1;//定时器重装
    CpuTimer0Regs.TCR.bit.TSS = 0;//启动定时器 
}  

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:Timer0中断处理函数				*/
/*------------------------------------------*/  
interrupt void Timer0_INT(void)
{
 	DINT;//关闭总中断
	CpuTimer0Regs.TCR.bit.TSS = 1;//停止定时器
	CpuTimer0Regs.TCR.bit.TIF = 1;//清零中断标志位
   	//if(temp==0x0100)
	//	temp=0x0001;
	LED_DAT=(LED_DAT^0x0800);
	//temp<<=1;
    PieCtrlRegs.PIEACK.all = 0x0001;   //响应中断
	CpuTimer0Regs.TCR.bit.TRB = 1;//定时器重装
    CpuTimer0Regs.TCR.bit.TSS = 0;//启动定时器
	EINT;   //开总中断
} 

 
