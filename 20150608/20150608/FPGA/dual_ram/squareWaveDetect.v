//use the fpga to detect the input square signal wave

module squareWaveDetect(
	input	iClk,			// input clock
	input	iRst_n,			//reset signal,low active
	input	isquareWave,	//the input square signal wave
	output	oState_n			//the square,low active,when the square wave signal is ok,this signal will be low.
	
	
);



//wire 	wDogEnable;
wire	wState_n;

squareWave squareWave_inst(
	.iClk(iClk),			// input clock
	.iRst_n(iRst_n),			//reset signal
	.isquareWave(isquareWave),	//the input square signal wave
	//.iDogEnable(wDogEnable),		//the dog module enable signal,high active
	.oState_n(wState_n)			//the square,
);
//assign	wDogEnable	=	1;//this signal will enable the dog,high active
assign	oState_n	=	wState_n;//the output signal,low active


endmodule
