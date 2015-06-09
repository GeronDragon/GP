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
//һ�¶����ַ��Χ����Ϊ�߽��Ҳ�����
//��������0x0-0x1f
//ż������0x100-0x11f
//���߾�Ϊ������������30���洢��Ԫ��
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
	input iRst_n;//ϵͳ��λ�ź�
	
	output iRst_a;//dsp a�ĸ�λ����ź�
	
	output oled1;//������������
	output oled2;//���յ�10����
	output oled3;//���յ�11����
	
	output state_a;//dsp_a��״̬��������״̬��������͵�ƽ��Ч
	output xint2_b;//��dsp b�����ж��ź�

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

	//��λ�źŵ���ʱ����ʱ�䳤�̴�������Ҫʱ����ʹ���ź�
	//�ж��źŵ���ʱ����ʱ�䳤�̴�������Ҫʱ����ʹ���ź�,�жϵ�λ���ڸ�λ֮��������ʱ��϶�?ȸ�λ�ĳ���Ҳ���Կ��������������һ��?	

	reg delayflag; //Ϊ1ʱ����������ʱ����
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
	
	wire wrstswd_a;//��λdsp_a��״̬������ģ��
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
	
	//����AʧЧ����state_a���������أ�ִ�С����л�xint2_b�ź���Դ��rxint2_b���ߵ�ƽ��֮ǰxint2_b���ȸߺ�ͼ�һ,���Լ���delay1
	//�ѱ�֤B���յ���Ч??
	//����AʧЧstate_a���������أ�ִ�С���delay1��??֤B����xint2_b?ź?��Դ��rxint2_b���ߵ�ƽ��
	//Ȼ����msAB��ͬʱд��
	//��������������
	//һ�ֽ����ʽ�ǽ�A��״??	//��һ��A��FPGA����FPGA�յ�����B���͵ڶ����ж��ź�
	//��
	
	//AʧЧ����state_a����delay1B���յ���Ч?��½��أ�Ȼ���л�xint2_b�ź���??��rxint2_b??�ߵ�ƽ��
	//Ȼ���͸�λ�źŸ�->�͡�������delay2A���յ���Ч���½��ء�Ȼ������A�ĸ�λ�źš�
	//delay3��֤A�ڸ�λ�������ȡ���ͷ����źŵĲ���
	//��A��λ�ɹ�
	//A��⵽��λ��ȡ���������FPGA����һ���źţ�FPGA�յ����B->�ͣ�1�ļ�������Ͳ���!state_a�ˡ�
	//delay4��֤B����??��Ч���½��غ�A�ܹ�FPGA�Ľ����Ȼ���л�xint2_b�ź�!state_a���ߵ�ƽ
	
	//Ϊ�˷�ֹ��λ֮���Ƿ�2��������state_a��������䣬��B�ٴη����ж��źš�
	//��ʱ��д�����
	
	//��A��λʧ��
	//�ղ���Axibf����0xint2_b�ź���Դ����Ϊrxint2_b��ʼ���Ǹߵ�??


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
	reg rSt;			//������֡ͷ�ı�־��1���������
	reg [5:0] rCount;
	reg [5:0] rDatacount;	
	reg [15:0] rOldData;
	reg oled1;//
	reg oled2;//��??��10??��??	
	reg oled3;//��11
	
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
				
				if ((rOldtempdata==16'h7e) && (rTempdata==16'h7e) && (rCount<3)) //���������г���������ox7e�����´���
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
