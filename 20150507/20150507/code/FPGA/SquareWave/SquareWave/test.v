`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   18:34:27 06/09/2015
// Design Name:   SquareWave
// Module Name:   C:/Users/Administrator/Documents/GitHub/GP/20150507/20150507/code/FPGA/SquareWave/SquareWave/test.v
// Project Name:  SquareWave
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: SquareWave
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_v;

	// Inputs
	reg iClk;
	reg isquareWave;
	reg iRst_n;

	// Outputs
	wire oState_n;

	// Instantiate the Unit Under Test (UUT)
	SquareWave uut (
		.iClk(iClk), 
		.isquareWave(isquareWave), 
		.iRst_n(iRst_n), 
		.oState_n(oState_n)
	);

	initial begin
		// Initialize Inputs
		iClk = 0;
		isquareWave = 0;
		iRst_n = 1;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
	
   always #16.67 iClk=~iClk;
	
	always 
	begin
		#2000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;		
		#200000 isquareWave=~isquareWave;
		#200000 isquareWave=~isquareWave;
		#200000 isquareWave=~isquareWave;
		#200000 isquareWave=~isquareWave;
		#200000 isquareWave=~isquareWave;
		#200000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#2000000 isquareWave=~isquareWave;
		#2000000 isquareWave=~isquareWave;
		#2000000 isquareWave=~isquareWave;
		#2000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		
//		#200000;
//		iRst_n = 0;
//		#200000;
//		iRst_n = 1;
//		#2000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;		
//		#200000 isquareWave=~isquareWave;
//		#200000 isquareWave=~isquareWave;
//		#200000 isquareWave=~isquareWave;
//		#200000 isquareWave=~isquareWave;
//		#200000 isquareWave=~isquareWave;
//		#200000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#2000000 isquareWave=~isquareWave;
//		#2000000 isquareWave=~isquareWave;
//		#2000000 isquareWave=~isquareWave;
//		#2000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
//		#1000000 isquareWave=~isquareWave;
	end      
endmodule

