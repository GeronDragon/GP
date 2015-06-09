//use the fpga to detect the input square signal wave

module squareWave(
	input	iClk,			// input clock--30Mhz
	input	iRst_n,			//reset signal
	input	isquareWave,	//the input square signal wave--500hz
	//input	iDogEnable,		//the dog module enable signal,high active
	output	oState_n			//the square,
	
	
);


	parameter	CRAZY_TIME	=	45000;//this count num will be based on the input clock,just count for the wight of the square wave,3ms 
//when the clock is 30MHz,it will delay 100ms,the dsp will start up in this time.
	parameter	START_TIME  =  30000;//when the system start up ,it will delay 100ms,then it begin to count the dog
	//在没达到这个启动时间之前，fpga都判断DSP A为正常，这个启动时间，主要用于防止DSP 和fpga的配置时间不同
	//比如，fpga先启动了，dsp尚未配置完全，这样，就有可能fpga本身gpio方波正常，但是fpga和dsp b均误判为dsp a故障，这样就会导致
	//组合逻辑判断为DSP A故障，从而DSP A读取到故障反馈信号后，再也无法正常启动了。
	//加入这个延时，就是为了解决这个问题
	reg			rState;
	reg			rState0;
	reg			rState1;
	reg[31:0]	rCnt;
	
	
	assign		oState_n	=	rState;
	always	@(posedge iClk)	
	begin		
		rState0	<=	isquareWave;
		rState1	<=	rState0;
	end
	
	
	//add 2014-2-26
//	reg rStartUpFlag;//the start up flag,when it is 0,the system is starting,1---the start up is ok
	reg[31:0]	rStartUpCnt;
	always @(posedge iClk or	negedge	iRst_n) 
	begin
		if(!iRst_n) begin
//			rStartUpFlag <= 0;
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
	//add 2014-2-26
	
	assign	cnt_en	=	(rState1	==	rState0);//cnt_en will be enable in every falling or rising edge
	
	always	@(posedge	iClk	or	negedge	iRst_n) 
	begin
			if(!iRst_n) 
			begin
			rCnt <= 32'd0;
			rState	<=	0;
			end 
			else if((!cnt_en) ) 
			begin
			rCnt	<=	32'd0;
			rState	<=	0;		//clear the output signal in every falling or rising edge
			end
			else if((rCnt	==	CRAZY_TIME) &&(rStartUpCnt == START_TIME)) 
			begin
			rState	<=	1;						//the counter is overflow 
			end
			else if((rCnt	==	CRAZY_TIME)) 
			begin
			rCnt <=CRAZY_TIME;
			end
			else 
			begin
			rCnt	<=	rCnt+1;
			end
	end
endmodule
