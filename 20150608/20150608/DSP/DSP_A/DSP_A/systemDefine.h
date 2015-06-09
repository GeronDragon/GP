#ifndef systemDefine_h
#define systemDefine_h

/****************常量宏定义*****************/ 

#define LENGTH 10	//需要备份的中间变量的数量（长度）
#define OK 1		//解包结果
#define ERR 0		//解包结果

#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)//edit by wzj
// DO NOT MODIFY THIS LINE.  
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

#endif

