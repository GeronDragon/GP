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
// Description: 实现了启动延时，检测500Hz方波的功能，仿真完成，实际未完成。若频率低于500，算为正常，若高于500，则输出高电平且保持不变，即使
// 方波恢复正常也还是高电平。所以在复位DSP A之后此功能需要重置。
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
	 //(目前clk频率为1MHz，而1500实测代表1.5ms,根据时钟的频率计算应该为1.5ms---hgl)
    parameter   START_TIME  =  30000;    
	 //在没达到这个启动时间之前，fpga都判断DSP A为正常，这个启动时间，主要用于防止DSP 和fpga的??    //比如，fpga先启动了，dsp尚未配置完全，这样，就有可?躥pga本身gpio方波正常?玣pga?蚫sp b均误判为dsp a故障，这样就会导致
    //组合逻辑判断为DSP A故障，从而DSP A读取到故障反馈信号后，再也无法正常启动了。
    //加入这个延时，就是为了解决这个问题
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
