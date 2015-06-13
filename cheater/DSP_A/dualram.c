#include "dualram.h"
#include "systemDefine.h"
#include "DSP281x_Device.h"
#include "System.h"


/************ȫ�ֱ���************************/
extern unsigned int buf[LENGTH+5]; 	//����������
extern unsigned int dat[LENGTH]; 		//���������


/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��XINTF						*/
/*------------------------------------------*/ 
void Init_XINTF(void)
{
	EALLOW;				
	XintfRegs.XTIMING6.bit.XWRLEAD=2;	//д��������ʱ��
	XintfRegs.XTIMING6.bit.XRDLEAD=2;	//����������ʱ��
	XintfRegs.XTIMING6.bit.XWRACTIVE=3;	//д������Чʱ��	
	XintfRegs.XTIMING6.bit.XRDACTIVE=2;	//��������Чʱ��
	XintfRegs.XTIMING6.bit.XRDTRAIL=2;	//����������ʱ��
	XintfRegs.XTIMING6.bit.XWRTRAIL=2;	//д��������ʱ��
	EDIS;
}



/*------------------------------------------*/
/*��ʽ������Adr--��ַ Data--����            */
/*����ֵ:void				                */
/*��������:��ָ����ַд��һ����������		*/
/*------------------------------------------*/ 
void Write_RAM(unsigned long Adr,unsigned int Data)
{
	unsigned int *pointer;
	pointer = (unsigned int*)(0x100000 | Adr);
	*pointer=Data;
}


/*------------------------------------------*/
/*��ʽ������Adr--��ַ            			*/
/*����ֵ: Data--����				    	*/
/*��������:��ָ����ַ����һ����������		*/
/*------------------------------------------*/  
unsigned int Read_RAM(unsigned long Adr)
{
	unsigned int *pointer;
	pointer = (unsigned int*)(0x100000 | Adr);
	return(*pointer);
} 


/*------------------------------------------*/
/*��ʽ������len--Ҫд��ĳ���				*/
/*			Adr--Ҫд��ĵ�ַ  				*/
/*����ֵ:void				                */
/*��������:��RAM��д������					*/
/*------------------------------------------*/ 
void WriteDat(unsigned int len,unsigned long Adr)
{
	unsigned int i;//���ݵ�ַ
	unsigned long temp=0x0;

	temp |= Adr;
	for (i=0;i<len;i++)
	{
		Write_RAM(temp++,buf[i]);
	}
} 


/*------------------------------------------*/
/*��ʽ������ָ��������RAM�׵�ַ			*/
/*����ֵ��У�����(OK����ERR)����Ч����		*/
/*��������������������ݽ��				*/
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
/*��ʽ������ָ�������������ָ��          */
/*����ֵ��void                              */
/*����������DSP A��������д��             */
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
/*��ʽ������ָ�������������ָ��          */
/*����ֵ��void                              */
/*����������DSP B��������д��             */
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


 
