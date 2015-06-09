#include "dualram.h"
#include "systemDefine.h"
#include "DSP281x_Device.h"
#include "System.h"


/************全局变量************************/
extern unsigned int buf[LENGTH+5]; 	//打包后的数组
extern unsigned int dat[LENGTH]; 		//待打包数组


/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化XINTF						*/
/*------------------------------------------*/ 
void Init_XINTF(void)
{
	EALLOW;				
	XintfRegs.XTIMING6.bit.XWRLEAD=2;	//写操作建立时间
	XintfRegs.XTIMING6.bit.XRDLEAD=2;	//读操作建立时间
	XintfRegs.XTIMING6.bit.XWRACTIVE=3;	//写操作有效时间	
	XintfRegs.XTIMING6.bit.XRDACTIVE=2;	//读操作有效时间
	XintfRegs.XTIMING6.bit.XRDTRAIL=2;	//读操作保护时间
	XintfRegs.XTIMING6.bit.XWRTRAIL=2;	//写操作保护时间
	EDIS;
}



/*------------------------------------------*/
/*形式参数：Adr--地址 Data--数据            */
/*返回值:void				                */
/*函数描述:向指定地址写入一个整型数据		*/
/*------------------------------------------*/ 
void Write_RAM(unsigned long Adr,unsigned int Data)
{
	unsigned int *pointer;
	pointer = (unsigned int*)(0x100000 | Adr);
	*pointer=Data;
}


/*------------------------------------------*/
/*形式参数：Adr--地址            			*/
/*返回值: Data--数据				    	*/
/*函数描述:从指定地址读出一个整型数据		*/
/*------------------------------------------*/  
unsigned int Read_RAM(unsigned long Adr)
{
	unsigned int *pointer;
	pointer = (unsigned int*)(0x100000 | Adr);
	return(*pointer);
} 


/*------------------------------------------*/
/*形式参数：len--要写入的长度				*/
/*			Adr--要写入的地址  				*/
/*返回值:void				                */
/*函数描述:向RAM中写入数据					*/
/*------------------------------------------*/ 
void WriteDat(unsigned int len,unsigned long Adr)
{
	unsigned int i;//数据地址
	unsigned long temp=0x0;

	temp |= Adr;
	for (i=0;i<len;i++)
	{
		Write_RAM(temp++,buf[i]);
	}
} 


/*------------------------------------------*/
/*形式参数：指向待解包的RAM首地址			*/
/*返回值：校验结论(OK或者ERR)与有效数据		*/
/*函数描述：将打包的数据解包				*/
/*------------------------------------------*/
unsigned int Unpack(unsigned long Adr)
{
	unsigned int t_result=OK;
	unsigned int len,i,des,r_data;

	unsigned int t_crc;
	unsigned long temp=0x0;

	temp |= Adr;

	r_data=Read_RAM(temp++);
	if (r_data!=0x7e)
	{
		t_result=ERR;
	}
	buf[0]=0x7e;
	r_data=Read_RAM(temp++);
	if (r_data!=0x7e) 
	{
		t_result=ERR;
	}
	buf[1]=0x7e;
	r_data=Read_RAM(temp++);
	des=r_data;
	buf[2]=des;
	r_data=Read_RAM(temp++);
	len=r_data;
	buf[3]=len;
	for (i=4;i<len+4;i++)
		buf[i]=Read_RAM(temp++);

	t_crc=Read_RAM(temp);
	if (CRC16(&buf[0],len+4,16)!=t_crc)
	{
		t_result=ERR;
	}

	return t_result;
}


/*------------------------------------------*/
/*形式参数：指向待打包的数组的指针          */
/*返回值：void                              */
/*函数描述：DSP A对数组进行打包             */
/*------------------------------------------*/
void A_Pack(unsigned int * ptr,unsigned int len)
{
	unsigned int * temp;
	unsigned int j;
	buf[0]=0x7e;
	buf[1]=0x7e;
	buf[2]=0x01;

	buf[3]=len;
	temp=ptr;
	for (j=0;j<len;j++,temp++)
	{
		buf[4+j]= * temp;
	}
	buf[4+len]=CRC16(&buf[0],len+4,16);
}

/*------------------------------------------*/
/*形式参数：指向待打包的数组的指针          */
/*返回值：void                              */
/*函数描述：DSP B对数组进行打包             */
/*------------------------------------------*/
void B_Pack(unsigned int * ptr,unsigned int len)
{
	unsigned int * temp;
	unsigned int j;
	buf[0]=0x7e;
	buf[1]=0x7e;
	buf[2]=0x02;

	buf[3]=len;
	temp=ptr;
	for (j=0;j<len;j++,temp++)
	{
		buf[4+j]= * temp;
	}
	buf[4+len]=CRC16(&buf[0],len+4,16);
}


 
