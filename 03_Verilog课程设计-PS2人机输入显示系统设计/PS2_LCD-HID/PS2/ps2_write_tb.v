`timescale 1ns/100ps 
module ps2_write_tb;
	reg	clk;
	reg	rst_n;
	reg	write;
	wire	error;
	wire	key_on;
	reg	[7:0]	write_data;
	wire	[7:0]	key_value;
	wire	tb_ps2_clk;
	wire	tb_ps2_data;
	
	reg	ps2_clk_r1;
	reg	ps2_clk_r2;
	reg	ps2_clk_r3;
	reg	key;
	reg	keyboard_clk;
	reg	tb_ps2_clk_link;
	reg	tb_ps2_data_link;
	reg	[2:0]  tb_state;
	reg	[1:0]	 tb_state2;
	reg	[3:0]	tb_num;
	reg	[9:0]	tb_data;
	reg	[7:0]	key_code;
	reg	[9:0] tb_key_code;
	reg	[11:0]	tb_50us_cnt;
	reg	[7:0]		tb_5us_cnt;
	reg	[10:0]	tb_15khz_cnt;
	reg   delay_50us_en;
	wire	delay_50us_done;
	wire  tb_ps2_clk_pose;
	reg   delay_5us_en;
	reg   tb_15khz_en;
	wire	tb_15khz_done;
	wire	delay_5us_done;
	
	parameter	time_50us = 12'd2400;
	parameter	time_5us  = 8'd240;
	parameter freq_15khz = 11'd1600;
	
	parameter	tb_idle = 3'd0;
	parameter	tb_tx = 3'd1;
	parameter	tb_wait  = 3'd2;
	parameter tb_delay = 3'd3;
	parameter tb_rx = 3'd4;
	parameter tb_ack = 3'd5;
	parameter tb_delay_5us = 3'd6;
	parameter tb_delay1 = 3'd7;
	parameter tb_idle2 = 2'd0;
	parameter tb_clk_l  =2'd1;
	parameter tb_clk_h = 2'd2;
	initial
	begin
	#0 clk = 1'b0;
	#0 keyboard_clk = 1'b0;
	#0 rst_n = 1'b1;
	#0 write_data = 8'b11110000;
	#0 key = 1'b0;
	#0 write = 1'b0;
	#0 key_code =8'h12;
	#20	rst_n = 1'b0;
	#100 rst_n = 1'b1;
	#10000 key = 1'b1;
	#101000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h12;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h58;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h58;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h12;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h1c;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'hf0;
	#100000 key = 1'b0;
	#1000000 key = 1'b1;
	#0 key_code = 8'h12;
	#100000 key = 1'b0;
	#1000000 write = 1'b1;
	#10000 write = 1'b0;
	#1000000 write = 1'b1;
	#10000 write = 1'b0;
	#3000000 $stop;
	end
	
	always #10 clk = ~clk;
	//always #32000 keyboard_clk = ~keyboard_clk;
	
	assign tb_ps2_clk = (tb_ps2_clk_link) ? keyboard_clk : 1'bZ;
	assign tb_ps2_data = (tb_ps2_data_link) ? 1'b0 : 1'bZ;
	wire parity = ~(^key_code);
	
	always @ (posedge clk or negedge rst_n)     
	begin
	if(~rst_n)
		begin
			ps2_clk_r1 <= 1'b1;
			ps2_clk_r2 <= 1'b1;
			ps2_clk_r3 <= 1'b1;
		end
	else
		begin
			ps2_clk_r1 <= tb_ps2_clk;
			ps2_clk_r2 <= ps2_clk_r1;
			ps2_clk_r3 <= ps2_clk_r2;
		end   	
	end
	
	assign tb_ps2_clk_pose = (~ps2_clk_r3 & ps2_clk_r2);   //      检测时钟线的上升沿;
	
	always @(posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		begin
		tb_state <= tb_idle;
		tb_ps2_clk_link <= 1'b0;
		tb_ps2_data_link <= 1'b0;
		tb_num <= 4'b0;
		tb_data <= 10'b0;
		end
	else
		case (tb_state)
		tb_idle:
				begin
			//	tb_ps2_clk_link   <= 1'b0;
				tb_ps2_data_link <= 1'b0;
				tb_num <= 4'b0;
				if(~tb_ps2_clk)
					tb_state <= tb_wait;
				else if(key)
					begin
					tb_key_code <= {1'b1,parity,key_code,1'b0};
					tb_state <= tb_tx;
					tb_ps2_data_link <= 1'b0;
					delay_5us_en <= 1'b1;	
					tb_state <= tb_delay_5us;				
					end
				end
				
		tb_delay_5us:
				begin
				if(delay_5us_done)
					begin
					tb_state <= tb_tx;
					delay_5us_en <= 1'b0;
					end
				end
				
		tb_wait:
				begin
				if(~tb_ps2_clk)
					begin
					delay_50us_en <= 1'b1;
					tb_state <= tb_delay;
					end
				else if(~tb_ps2_data)
					tb_state	 <= tb_rx;
				end
				
		tb_delay:
				begin
				if(delay_50us_done)
					begin
					tb_state <= tb_wait;
					delay_50us_en <= 1'b0;
				    end
				end
				
		tb_rx:
				begin
			//	tb_ps2_clk_link <= 1'b1;
				if(tb_ps2_clk_pose )
				begin
					if(tb_num<4'd10)
						begin
						tb_data <= {tb_ps2_data,tb_data[9:1]};
						tb_num <= tb_num + 4'd1;
						tb_state <= tb_rx;
						end
					else if(tb_num == 4'd10)
						begin
						tb_state <= tb_ack;
						tb_num <= 4'd0;
						end	
				end
				end
		tb_ack:
				begin
				delay_5us_en <= 1'b1;
				if(delay_5us_done)
				tb_ps2_data_link <= 1'b1;		
				else if(tb_ps2_clk_pose)
					begin
					delay_50us_en <= 1'b1;
					tb_state <= tb_delay1;
					end
				end 
				
		tb_delay1:	
				begin
				tb_ps2_clk_link<= 1'b0;
				if(delay_50us_done)
					begin
					tb_state <= tb_idle;
					delay_50us_en <= 1'b0;
				    end
				end
		tb_tx:
				begin
					if(tb_ps2_clk_pose)
						delay_5us_en <= 1'b1;
					else if(delay_5us_done )
						begin
							if( tb_num < 4'd11)
							begin
							delay_5us_en <= 1'b0;
							tb_num <= tb_num + 1'b1;
							tb_ps2_data_link <= ~tb_key_code[0];
							tb_key_code <= {1'b0,tb_key_code[9:1]};
							end	
					 	else if(tb_num == 4'd11)
							begin
							tb_ps2_data_link <= 1'b0;
							tb_state <= tb_idle;
							end
						end	
				end
		default: tb_state <= tb_idle;
		endcase
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			begin
			tb_state2 <= tb_idle2;
			tb_15khz_en <= 1'b0;
			end
		else
			case (tb_state2)
			tb_idle2:
					begin
					tb_ps2_clk_link <= 1'b0;
						if(tb_state == tb_tx)
							tb_state2 <= tb_clk_h;
						else if(tb_state == tb_rx)
							tb_state2 <= tb_clk_h;
					end
			tb_clk_l:
					begin
						tb_ps2_clk_link <= 1'b1;
						keyboard_clk <= 1'b0;
						if(~tb_15khz_done)
							tb_15khz_en <= 1'b1;
						else
							begin
							tb_15khz_en <= 1'b0;
							tb_state2 <= tb_clk_h;
							end
					end	
			tb_clk_h:
					begin					
						keyboard_clk <= 1'b1;
						if(~tb_15khz_done)
							begin
							tb_ps2_clk_link <= 1'b1;
							tb_15khz_en <= 1'b1;
							end
						else if(tb_state == tb_tx || tb_state == tb_rx || tb_state == tb_ack)
							begin
							tb_ps2_clk_link <= 1'b1;
							tb_15khz_en <= 1'b0;
							tb_state2 <= tb_clk_l;
							end
						else
							begin
							tb_state2 <= tb_idle2;
							tb_ps2_clk_link <= 1'b0;
							end
					end
			default:tb_state2 <= tb_idle2;
			endcase	
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			tb_15khz_cnt <= 11'b0; 
		else if(~tb_15khz_en)
			tb_15khz_cnt <= 11'b0;
		else if(~tb_15khz_done )
			tb_15khz_cnt <= tb_15khz_cnt + 1'b1;
		else 
			tb_15khz_cnt <= 11'b0;
	end
	assign tb_15khz_done = (tb_15khz_cnt == freq_15khz);
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			tb_5us_cnt <= 8'b0; 
		else if(~delay_5us_en)
			tb_5us_cnt <= 8'b0;
		else if(~delay_5us_done )
			tb_5us_cnt <= tb_5us_cnt + 1'b1;
		else 
			tb_5us_cnt <= 8'b0;
	end
	
	assign delay_5us_done = (tb_5us_cnt == time_5us);
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			tb_50us_cnt <= 12'b0; 
		else if(~delay_50us_en)
			tb_50us_cnt <= 12'b0;
		else if(~delay_50us_done )
			tb_50us_cnt <= tb_50us_cnt + 1'b1;
		else
			tb_50us_cnt <= 12'b0;
	end
	
	assign delay_50us_done = (tb_50us_cnt == time_50us);
	
 ps2 U1(
	.clk(clk),
	.rst_n(rst_n),
	.ps2_data(tb_ps2_data),
	.ps2_clk(tb_ps2_clk),
	.write(write),
	.error(error),
	.key_on(key_on),
	.write_data(write_data),
	.key_value(key_value)
	);
endmodule