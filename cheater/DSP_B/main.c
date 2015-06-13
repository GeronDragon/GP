/******************************************************************/
/*-------------------20150502-LONGREJOY---------------------------*/        
/******************************************************************/
//A_GPIO_CHECK  62  C5TRIP#/GPIOB14
//A_GPIOA11 可用于连接LED作为测试结果输出
/*****************头文件********************/  
#include "DSP281x_Device.h"
#include "System.h"

#include "systemCrc16.h"
#include "systemInterrupt.h"
#include "systemExternalInterrupt.h"
#include "dualram.h"

#include "systemDefine.h"

/*因为可以利用外部的1MS中断进行IO口翻转从而得到500Hz状态脉冲，故不用定时器0进行计时也可以*/



/***************全局变量定义****************/  
unsigned int buf[LENGTH+5]; 	//打包后的数组
unsigned int dat[LENGTH]; 		//待打包数组
unsigned int Parity=1;			//写入奇偶次标志位
unsigned int flag=0;			//测试用标志，用于停止/启动发送数据，若1则开始，0则停止
//unsigned int flag=1;			//测试用


/****************函数声明*******************/ 
void Init_LED(void);
//void Init_Timer0(void);
//interrupt void Timer0_INT(void); 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:主函数			                */
/*------------------------------------------*/ 
void main(void)
{
	//int 16b  long 32b
	//unsigned int i,odd_des,even_des;
	//unsigned long temp;
	//unsigned int FirSt;	//标志位。若为第一次上电则为1；若为后续复位则为0
	unsigned int Readflag=0; //读取标志位，读取后置1，以后不再读取
	unsigned int i,j;

	//初始化
	InitSysCtrl();	 	//系统初始化子程序，在DSP28_sysctrl.c中，关了总中断
	Init_LED();	 		//初始化状态LED		A11
	Init_XINTF();	 	//初始化XINTF的Zone6
	systemExternalInterruptInit();
	systemInterruptEnable();	//初始化外部中断，开了总中断，//l.r.20150505开中断语句被注释了
	//先关闭INT1，等待INT2的信号之后再开
	XIntruptRegs.XINT1CR.all=0x0;//关闭管脚xint1外中断，默认是下降沿触发中断，如为上升沿，则0x5

	EINT;   // Enable Global interrupt INTM				//l.r.20150505此时开中断
    ERTM;   // Enable Global realtime interrupt DBGM		//l.r.20150505此时开中断
	//灭个灯代表程序正在运行
	LED_DAT &= ~0x0C00;
	while(1)
	{	
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		//灭1s
		for (i=0;i<1000;i++)
			for (j=0;j<4;j++)
				DELAY_US(250);
		//亮1s
		LED_DAT |= 0x0800;
		for (i=0;i<1000;i++)
			for (j=0;j<4;j++)
				DELAY_US(250);
		//灭
		LED_DAT &= ~0x0800;
	}

} 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化状态LED端口为输出并拉低A8  */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
	LED_DAT |= 0x0800;	//先拉高作为初始化
}   



 
