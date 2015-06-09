/******************************************************************/
/*-------------------20150426-LONGREJOY---------------------------*/        
/******************************************************************/
//A_GPIO_CHECK  62  C5TRIP#/GPIOB14
//A_GPIOA11 可用于连接LED作为测试结果输出
//A_GPIOA10 用于向FPGA发送复位成功信号
/*****************头文件********************/  
#include "DSP281x_Device.h"
#include "System.h"

#include "systemCrc16.h"
#include "systemInterrupt.h"
#include "systemExternalInterrupt.h"
#include "dualram.h"

#include "systemDefine.h"



/*因为可以利用外部的1MS中断进行IO口翻转从而得到500Hz状态脉冲，故不用定时器0进行计时也可以*/

/****************端口宏定义*****************/
#define StaPul_DIR GpioMuxRegs.GPBDIR.all		//状态脉冲发生口方向寄存器
#define StaPul_DAT GpioDataRegs.GPBDAT.all		//状态脉冲发生口数据寄存器

#define LED_DIR GpioMuxRegs.GPADIR.all		//状态脉冲发生口方向寄存器
#define LED_DAT GpioDataRegs.GPADAT.all		//状态脉冲发生口数据寄存器

#define Check_DIR GpioMuxRegs.GPADIR.all		//信号发生口方向寄存器
#define Check_DAT GpioDataRegs.GPADAT.all		//信号发生口数据寄存器

/***************全局变量定义****************/  
unsigned int buf[LENGTH+5]; 	//打包后的数组
unsigned int dat[LENGTH]; 		//待打包数组
unsigned int Parity=1;			//写入奇偶次标志位
unsigned int flag=0;			//测试用标志，用于停止发送数据


/****************函数声明*******************/ 
void Init_StaPul(void);
void Init_LED(void);
void Init_Check(void);


/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:主函数			                */
/*------------------------------------------*/ 
void main(void)
{
	unsigned int i,odd_des,even_des;
	unsigned int FirSt;	//标志位。若为第一次上电则为1；若为后续复位则为0

	//初始化
	InitSysCtrl();	 //系统初始化子程序，在DSP28_sysctrl.c中，关了总中断
	Init_StaPul();	 //初始化状态脉冲
	Init_LED();
	Init_Check();
	Init_XINTF();	 //初始化XINTF的Zone6
	systemExternalInterruptInit();
	systemInterruptEnable();	//初始化外部中断，开了总中断	//l.r.20150505开中断语句被注释了
	
	for (i=0;i<LENGTH;i++)
		dat[i]=0;

	//先读入两次RAM中读取DES位判断是否是第一次上电
	//奇数次odd写入0x100000开始的LENGTH长空间
	//偶数次even写入0x140000开始的LENGTH长空间
	odd_des=Read_RAM(0x3);//20150515 l.r. 改为从0x1位置开始写入第一个数据
	even_des=Read_RAM(0x103);//20150515 l.r. 改为从0x40001位置开始写入第一个数据
	if ((odd_des==0) && (even_des==0))
		FirSt=1;
	else
		FirSt=0;
	//此处判断依据也可以是非0、1，主要看上电FPGA的IP核内容是什么，是0还是65535
	
	//复位上电的处理逻辑
	if (FirSt==0)
	{
		//解包奇数区域
		if (Unpack(0x1)==OK)		//解包奇数区域正确
		{
			for (i=0;i<LENGTH;i++)
				dat[i]=buf[i+4];	//RAM中数据已经在buff中，将其中的数据段提取到dat中，作为中间变量
			//继续运行算法
			//将得到的新中间变量写入RAM中
			//向FPGA发送复位成功的信号
			Check_DAT &= ~0x0400;
		}
		//解包偶数区域
		else if (Unpack(0x101)==OK)	//解包偶数区域正确
		{
			for (i=0;i<LENGTH;i++)
				dat[i]=buf[i+4];	//RAM中数据已经在buff中，将其中的数据段提取到dat中，作为中间变量
			//继续运行算法
			//将得到的新中间变量写入RAM中
			//向FPGA发送复位成功的信号
			Check_DAT &= ~0x0400;
		}

	}
	//第一次上电的处理逻辑，与复位上电后从RAM中提取完中间数据后操作一致，所以没有else分支
	//else 
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	DELAY_US(250);
	EINT;   // Enable Global interrupt INTM           	//l.r. 20150505在此处开
    ERTM;   // Enable Global realtime interrupt DBGM	//l.r. 20150505在此处开
	//灭个灯代表程序正在运行
	LED_DAT &= ~0x0800;

	while(1)
	{
		//运行算法---此处因为算法简单放到1ms中断中去了
		if (flag==1)
			XIntruptRegs.XINT1CR.all=0x0;//关闭管脚xint1外中断
	}

} 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化状态脉冲端口为输出并拉低   */
/*------------------------------------------*/ 
void Init_StaPul(void)
{
	EALLOW;
	StaPul_DIR |= 0x4000;
	EDIS;
	StaPul_DAT &= ~0x4000;	//先拉低状态脉冲口作为初始化
}  

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化向FPGA发送信号的端口A10    */
/*------------------------------------------*/ 
void Init_Check(void)
{
	EALLOW;
	Check_DIR |= 0x0400;
	EDIS;
	Check_DAT |= 0x0400;	//先拉高作为初始化
}   
 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化状态LED端口为输出并拉低A11 */
/*------------------------------------------*/ 
void Init_LED(void)
{
	EALLOW;
	LED_DIR |= 0x0800;
	EDIS;
	LED_DAT |= 0x0800;	//先拉高作为初始化
}   



 
