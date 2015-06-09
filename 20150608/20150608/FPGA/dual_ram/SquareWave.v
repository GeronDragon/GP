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
	//��û�ﵽ�������ʱ��֮ǰ��fpga���ж�DSP AΪ�������������ʱ�䣬��Ҫ���ڷ�ֹDSP ��fpga������ʱ�䲻ͬ
	//���磬fpga�������ˣ�dsp��δ������ȫ�����������п���fpga����gpio��������������fpga��dsp b������Ϊdsp a���ϣ������ͻᵼ��
	//����߼��ж�ΪDSP A���ϣ��Ӷ�DSP A��ȡ�����Ϸ����źź���Ҳ�޷����������ˡ�
	//���������ʱ������Ϊ�˽���������
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
