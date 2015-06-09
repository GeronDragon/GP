`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:35:44 04/08/2015 
// Design Name: 
// Module Name:    SquareWave 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: ʵ����������ʱ�����500Hz�����Ĺ��ܣ�������ɣ�ʵ��δ��ɡ���Ƶ�ʵ���500����Ϊ������������500��������ߵ�ƽ�ұ��ֲ��䣬��ʹ
// �����ָ�����Ҳ���Ǹߵ�ƽ�������ڸ�λDSP A֮��˹�����Ҫ���á�
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module SquareWave(
    input   iClk,           // input clock--30Mhz
    input   isquareWave,    //the input square signal wave--500hz
	 input 	iRst_n,			 //reset signal
    output  oState_n        //the square,    
);

    parameter   CRAZY_TIME  =   45000;
	 //(ĿǰclkƵ��Ϊ1MHz����1500ʵ�����1.5ms,����ʱ�ӵ�Ƶ�ʼ���Ӧ��Ϊ1.5ms---hgl)
    parameter   START_TIME  =  30000;    
	 //��û�ﵽ�������ʱ��֮ǰ��fpga���ж�DSP AΪ�������������ʱ�䣬��Ҫ���ڷ�ֹDSP ��fpga��??    //���磬fpga�������ˣ�dsp��δ������ȫ�����������п�?�fpga����gpio��������?���fpga?�dsp b������Ϊdsp a���ϣ������ͻᵼ��
    //����߼��ж�ΪDSP A���ϣ��Ӷ�DSP A��ȡ�����Ϸ����źź���Ҳ�޷����������ˡ�
    //���������ʱ������Ϊ�˽���������
    reg         rState=1;
    reg         rState0=1;
    reg         rState1=1;
    reg[31:0]   rCnt=0;
    reg[31:0]   rStartUpCnt=0;
    
    assign      oState_n    =   rState;
	 
    always  @(posedge iClk) 
	 begin        
        rState0 <=  isquareWave;
        rState1 <=  rState0;
    end
    
	 always @(posedge iClk or negedge iRst_n) 
	 begin
		  if (!iRst_n)
		  begin
				rStartUpCnt<=0;
		  end
		  else if (rStartUpCnt == START_TIME) 
		  begin
            rStartUpCnt <= START_TIME;
        end
        else    
		  begin
            rStartUpCnt <= rStartUpCnt+1;
        end             
    end
    
    assign  cnt_en  =   (rState1    ==  rState0);//cnt_en will be enable in every falling or rising edge
    
	 always  @(posedge   iClk or	negedge	iRst_n) 
	 begin
			if(!iRst_n) 
			begin
				rCnt <= 32'd0;
				rState	<=	0;
			end 
			else if (!(rStartUpCnt == START_TIME))
			begin
				rState  <=  0; 
			end
			else if((!cnt_en) && (!(rCnt   ==  CRAZY_TIME))) 
			begin
            rCnt    <=  32'd0;
            rState  <=  0;      //clear the output signal in every falling or rising edge
         end
			else	if((rCnt   ==  CRAZY_TIME) && (rStartUpCnt == START_TIME)) 
			begin
            rState  <=  1;                      //the counter is overflow 
         end
         else if((rCnt   ==  CRAZY_TIME)) 
			begin
            rCnt <=CRAZY_TIME;
         end
			else 
			begin
            rCnt    <=  rCnt+1;
         end
     end
	  
endmodule
