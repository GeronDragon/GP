//###########################################################################
//
// FILE:	systemExternalInterrupt.c
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
//外部1ms定时中断初始化   XINT1 CPU INT1  XINT1 in the PIE: Group 1 interrupt 4
//上电默认0 Interrupt generated on a falling edge (high-to-low transition)


extern unsigned int dat[LENGTH]; 		//待打包数组
extern unsigned int buf[LENGTH+5]; 		//打包后的数组
extern unsigned int Parity;//标志位。若为1则写入奇数区，若为0则写入偶数区
extern unsigned int flag;			//测试用标志，用于停止发送数据,0停1发

unsigned int INT2FLAG=0;//标志位，奇数次的INT2则为1，偶数次则为0

void systemExternalInterruptInit(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers
   	PieVectTable.XINT1 = &system_external_interrupt0_isr;//指定外部1ms中断服务子程序
	

	PieVectTable.XINT2 = &system_external_interrupt1_isr;//指定xint2 外部1ms中断服务子程序
   	EDIS;    // This is needed to disable write to EALLOW protected registers

}

//xint1 对应外部1ms中断，用于更新数据与IO口翻转
interrupt void system_external_interrupt0_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
/* #if EXTERNAL_INTERRUPT_SELECT == 0 */
 	systemExternalInterruptHandle();
/* 	                                  */
/* #endif                             */
}


//xint2 对应于fpga中产生的中断，用于指令获取
interrupt void system_external_interrupt1_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
 	systemExternalInterrupt2Handle();  
}



//F2812在外部1ms定时中断处理函数里面对这个函数进行调用
//xint1 对应外部1ms中断，用于读取数据与检测算法是否连续
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
			
	//将得到的新中间变量写入RAM中
	if (Parity)
	//打包并写入奇数区
	{
		B_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x1);
		Parity=0;
	}
	//打包并写入偶数区
	else
	{
		B_Pack(&dat[0],LENGTH);
		WriteDat(LENGTH+5,0x101);
		Parity=1;
	}
	
}


//xint2 对应于fpga中产生的中断，用于指令获取
void systemExternalInterrupt2Handle(void)
{
	//unsigned int i;
	INT2FLAG=!INT2FLAG;
	if (INT2FLAG) //奇数次
	{
		flag=1;
		//if ((dat[0]==0) || (dat[0]==9))
		//点个灯表示进入中断了
//			LED_DAT |= 0x0800;
//		else
//			LED_DAT &= ~0x0800;
	}
	else //偶数次
	{
		flag=0;
		//LED_DAT &= ~0x0800;
	}
}

//如果将决策留给FPGA判断，则xint2就不用使用了
  

