#ifndef dualram_h
#define dualram_h



void Init_XINTF(void);
void Write_RAM(unsigned long Adr,unsigned int Data);
unsigned int Read_RAM(unsigned long Adr);
void WriteDat(unsigned int len,unsigned long Adr);
unsigned int Unpack(unsigned long Adr);
void A_Pack(unsigned int * ptr,unsigned int len);
void B_Pack(unsigned int * ptr,unsigned int len);

#endif

