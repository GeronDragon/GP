`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:19:31 04/12/2015 
// Design Name: 
// Module Name:    dual_ram 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
//一下定义地址范围，均为边界且不包含
//奇数区域：0x0-0x1f
//偶数区域：0x100-0x11f
//两边均为不包含，即各30个存储单元。
`define ODD_RAM_BEGIN 10'h0
`define ODD_RAM_END 10'h1f
`define EVEN_RAM_BEGIN 10'h100
`define EVEN_RAM_END 10'h11f

module dual_ram(clk,xzcs6_a,xzcs6_b,addr_a,data_a,addr_b,data_b,we_a,we_b,rd_a,rd_b,oled1,oled2,oled3,squarewave_a,state_a,xint2_b,iRst_n,iRst_a);

	input clk;
	input xzcs6_a;
	input xzcs6_b;
	input [9:0] addr_a;
	inout [15:0] data_a;
	input [9:0] addr_b;
	inout [15:0] data_b;
	input we_a;
	input we_b;
	input rd_a;
	input rd_b;
	input squarewave_a;
	input iRst_n;//系统复位信号
	
	output iRst_a;//dsp a的复位结果信号
	
	output oled1;//数据连续则点灯
	output oled2;//接收到10则点灯
	output oled3;//接收到11则点灯
	
	output state_a;//dsp_a的状态脉冲有无状态的输出，低电平有效
	output xint2_b;//给dsp b发送中断信号

	wire [15:0] data_a;	
	wire [15:0] data_b;
	
	wire [15:0] wDataOut_a;
	wire [15:0] wDataOut_b;
	
	dual_ram_core mydualram(
	.addra(addr_a),
	.addrb(addr_b),
	.clka(clk),
	.clkb(clk),
	.dina(data_a),
	.dinb(data_b),
	.douta(wDataOut_a),
	.doutb(wDataOut_b),
	.ena(xzcs6_a),
	.enb(xzcs6_b),
	.wea(we_a),
	.web(we_b)
	);

	assign data_a = ((!rd_a) && (!xzcs6_a))? wDataOut_a : 16'hzzzz;
	assign data_b = ((!rd_b) && (!xzcs6_b))? wDataOut_b : 16'hzzzz;

	//复位信号的延时程序，时间长短待定，必要时引入使能信号
	//中断信号的延时程序，时间长短待定，必要时引入使能信号,中断的位置在复位之后，所以其时间肯定?雀次坏某ぁＲ部梢钥悸橇礁龀绦蚝显谝黄稹?	

	reg delayflag; //为1时用于启动延时计数
	always @(posedge state_a or negedge iRst_n)
	begin
		if (!iRst_n)
		begin
			delayflag=0;
		end
		else
		begin
			delayflag=1;
		end
	end
	
	reg [17:0] delaycounter;
	always @(posedge clk or negedge iRst_n)
	begin
		if (!iRst_n)
		begin
			delaycounter=0;
		end
		else
		begin
			if (delayflag==1)
				delaycounter=delaycounter+1;
		end
	end
	
	wire wrstswd_a;//复位dsp_a的状态脉冲检测模块
	reg rstswd_a;
	reg cf;
	reg iRst_a;
	reg xint2_b;
	
	always @ (posedge clk or negedge iRst_n)
	begin
		if (!iRst_n)
		begin
			rstswd_a=1;
			xint2_b = 1;
			cf = 1;
		end
		else 
		begin
			if (state_a==1)
			begin
				if (cf==1)
				begin
					iRst_a=0;
					xint2_b=0;
					rstswd_a=0;
				end
				if (delaycounter==15000)
				begin
					cf=0;
					iRst_a=1;
					xint2_b=1;
					rstswd_a=1;
				end
			end
			if (delaycounter==1050000)
			begin
				begin
					if (state_a==0)
					begin
						xint2_b=0;
					end
				end
			end
		end
	end
	
	//假设A失效，则state_a出现上升沿，执行。先切换xint2_b信号来源到rxint2_b即高电平，之前xint2_b是先高后低即一,所以加入delay1
	//已保证B能收到有效??
	//假设A失效state_a出现上升沿，执行。先delay1，??证B能收xint2_b?藕?来源到rxint2_b即高电平。
	//然后发送msAB会同时写入
	//必须避免这种情况
	//一种解决方式是将A的状??	//另一种A检FPGA发送FPGA收到后向B发送第二次中断信号
	//建
	
	//A失效，则state_a出现delay1B能收到有效?南陆笛兀缓笄谢粁int2_b信号来??到rxint2_b??高电平。
	//然后发送复位信号高->低。再利用delay2A能收到有效的下降沿。然后拉高A的复位信号。
	//delay3保证A在复位后能完成取数和发送信号的操作
	//若A复位成功
	//A检测到复位且取数正常后给FPGA发送一个信号，FPGA收到后给B->低，1的检测条件就不是!state_a了。
	//delay4保证B能收??有效的下降沿和A能够FPGA的结果，然后切换xint2_b信号!state_a即高电平
	
	//为了防止复位之后还是发2出加入检测state_a的条件语句，给B再次发送中断信号。
	//暂时不写这个异
	
	//若A复位失败
	//收不到Axibf保持0xint2_b信号来源保持为rxint2_b，始终是高电??


	squareWaveDetect squareWave_inst(
	.iClk(clk),
	.isquareWave(squarewave_a),
	.iRst_n(wrstswd_a),
	.oState_n(state_a)
	);
	assign wrstswd_a=rstswd_a;
	
	reg [9:0] rOldAddr;
	reg [9:0] rTempaddr;
	reg [15:0] rTempdata;
	reg [15:0] rOldtempdata;
	reg [15:0] stream [0:31];//
	reg rSt;			//检索到帧头的标志。1代表检索到
	reg [5:0] rCount;
	reg [5:0] rDatacount;	
	reg [15:0] rOldData;
	reg oled1;//
	reg oled2;//接??到10??点??	
	reg oled3;//接11
	
	always @ (posedge clk or negedge	iRst_n)
	begin
	if(!iRst_n) 
		begin
			rTempdata=16'h0;
			rOldtempdata=16'h0;
			rTempaddr=10'h0;
			rOldAddr=10'h0;
			oled1=1'h0;
			oled2=1'h0;
			oled3=1'h0;
			stream[0]=16'h0;stream[1]=16'h0;stream[2]=16'h0;stream[3]=16'h0;
			stream[4]=16'h0;stream[5]=16'h0;stream[6]=16'h0;stream[7]=16'h0;
			stream[8]=16'h0;stream[9]=16'h0;stream[10]=16'h0;stream[11]=16'h0;
			stream[12]=16'h0;stream[13]=16'h0;stream[14]=16'h0;stream[15]=16'h0;
			rSt=1'h0;
			rCount=6'h0;
			rDatacount=6'h0;
			rOldData=16'h0;
		end
	else
		begin
			if ((!we_a) && (!xzcs6_a) && (((addr_a>`ODD_RAM_BEGIN) && (addr_a<`ODD_RAM_END)) || ((addr_a>`EVEN_RAM_BEGIN) && (addr_a<`EVEN_RAM_END))))
			begin
				rTempdata=data_a;
				rTempaddr=addr_a;
			end
			if ((!we_b) && (!xzcs6_b) && (((addr_b>`ODD_RAM_BEGIN) && (addr_b<`ODD_RAM_END)) || ((addr_b>`EVEN_RAM_BEGIN) && (addr_b<`EVEN_RAM_END))))
			begin
				rTempdata=data_b;
				rTempaddr=addr_b;
			end
			
			if ((rTempaddr!=rOldAddr) && (((rTempaddr>`ODD_RAM_BEGIN) && (rTempaddr<`ODD_RAM_END)) || ((rTempaddr>`EVEN_RAM_BEGIN) && (rTempaddr<`EVEN_RAM_END))))
			begin			
				rOldAddr=rTempaddr;
				
				if ((rOldtempdata==16'h7e) && (rTempdata==16'h7e) && (rCount<3)) //避免数据中出现连续的ox7e而导致错误
				begin
					rSt=1;
				end
				else 
				begin
					rSt=0;
				end
				
				rOldtempdata=rTempdata;
				
				if (rSt==1)
				begin
					stream[0]=16'h7e;
					rCount=1;
					stream[rCount]=rTempdata;
					rCount=rCount+1;
				end
				if (rSt==0)
				begin
					stream[rCount]=rTempdata;
				
					if (rCount==3)
					begin
						rDatacount=rTempdata;
					end
					if (rCount==4)
					begin
						if ((rTempdata-rOldData)==1)
						begin
							oled1=1;
						end
						else
						begin
							oled1=0;
						end
						rOldData=rTempdata;
					end
					
					if ((stream[rCount]==16'ha) && (rCount>3) && (rCount<(rDatacount+4)))
					begin
						oled2=1;
					end
					
					if ((stream[rCount]==16'hb) && (rCount>3) && (rCount<(rDatacount+4)))
					begin
						oled3=1;
					end
					if (rCount==(rDatacount+4))
					begin
						rCount=0;
					end
					
					rCount=rCount+1;
				end		
			end
		end
	end
	
	
endmodule
