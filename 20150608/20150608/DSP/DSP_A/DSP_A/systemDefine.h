#ifndef systemDefine_h
#define systemDefine_h

/****************�����궨��*****************/ 

#define LENGTH 10	//��Ҫ���ݵ��м���������������ȣ�
#define OK 1		//������
#define ERR 0		//������

#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)//edit by wzj
// DO NOT MODIFY THIS LINE.  
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

#endif

