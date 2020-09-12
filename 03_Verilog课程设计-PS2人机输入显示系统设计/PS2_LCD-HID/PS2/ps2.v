module ps2(
	clk,
	rst_n,
	ps2_data,
	ps2_clk,
	write,
	error,
	key_on,
	write_data,
	shift,
	capslock,
	key_value,
	relase,
	speed
	);
	
	input				clk;                    // 与VGA时钟一样是25MHZ;
	input				rst_n;      
	inout				ps2_clk;                //  PS2时钟线;
	inout				ps2_data;               //  PS2数据线;
	input				write;
	input		[7:0]	write_data;
	output			error;
	output			key_on;
	output	[7:0]	key_value;              //  键盘键值;
	output			shift;
	output			capslock;
	output			relase;
	output			speed;
	
	reg				capslock;
	reg		[7:0]	key_value;
	reg				key_on;                 //  有键按下信号;
	reg				key_onr;
	reg				key_onrr;
    reg				relase;                 //  有键释放信号;
    reg				ps2_clk_link;
    reg				ps2_data_link;
	reg				ps2_clk_r1;
	reg				ps2_clk_r2;
	reg				ps2_clk_r3;
	reg	[2:0]		state;
	reg	[3:0]		num;
	reg	[9:0]	data;
	reg	[7:0] 		data1;
	reg	[7:0] 		data2;
	reg	[7:0] 		data3;
	reg	[9:0]		send_data;
	reg	[12:0]	time_120us_cnt;
	reg	[12:0]	time_100us_cnt;
	reg	[7:0]		time_5us_cnt;
	reg				time_120us_en;
	reg				time_5us_en;
	reg				ack;
	reg				error;
	
	reg				shift_right;
	reg				shift_left;
	reg				capslock_key;
	reg				capslock_r;
	
	
	
	wire				ps2_clk_fall;
	wire				ps2_clk_pose;
	wire				time_120us_done;
	wire				time_100us_done;
	wire				time_5us_done;
	wire				parity;
	wire				shift;
	wire				caps_sw;
	
	parameter     RIGHT_SHIFT = 8'h59;
	parameter     LEFT_SHIFT = 8'h12;
	parameter		CAPS            = 8'h58;
	parameter     TIME_120US = 13'd5760;
	parameter     TIME_100US = 13'd4800;
	parameter     TIME_5US     =  8'd240;
    parameter 	  	IDLE =3'd0; 
	parameter   	REV  = 3'd1;
	parameter 		FORCE_CLK_L = 3'd2;
	parameter     TX = 3'd3;
	parameter     ACK = 3'd4;
	parameter		CHECK = 3'd5;
	parameter		WAIT	= 3'd6;
	


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
			ps2_clk_r1 <= ps2_clk;
			ps2_clk_r2 <= ps2_clk_r1;
			ps2_clk_r3 <= ps2_clk_r2;
		end   	
	end
	
	assign ps2_clk_fall = (ps2_clk_r3 & ~ps2_clk_r2);   //      检测时钟线的下降沿;
	assign ps2_clk_pose = (~ps2_clk_r3 & ps2_clk_r2);   //      检测时钟线的上升沿;
	
	assign ps2_clk   = ps2_clk_link ? 1'b0:1'bZ  ;
	assign ps2_data = ps2_data_link ? 1'b0:1'bZ  ;
	assign parity = ~(^write_data);
	assign speed = (state == WAIT&& num > 4'd5);
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		begin
		state <= IDLE;
		num <= 4'b0;
		data <= 8'b0; 
		send_data <=10'b0;
		ps2_clk_link <= 1'b0;
		ps2_data_link <= 1'b0;
		time_120us_en <= 1'b0;
		time_5us_en <= 1'b0;
		error <= 1'b0;
		ack <= 1'b1;
		end
	else 
	case(state)
	IDLE:
			begin
			ps2_clk_link   <= 1'b0;
			ps2_data_link <= 1'b0;
			num <= 4'b0;
			ack <= 1'b1;
			if(write)
				state <= FORCE_CLK_L;
			else if(ps2_clk_fall)
				state <= REV;
			else
				state <= IDLE;
			end
	REV: 
			begin
				ps2_clk_link   <= 1'b0;
				ps2_data_link <= 1'b0;
				if(ps2_clk_fall && num <4'd10)
					begin
					data <= {ps2_data,data[9:1]};
					num <= num + 4'd1;
					state <= REV;
					end
				else if(num == 4'd10)
					begin
					state <= WAIT;
					num <= 4'd0;
					end	
			end
	WAIT: 	
			begin
			if(num == 4'd10)
				state <= IDLE;
			else 
				begin
				num <= num + 1'b1;
				state <= WAIT;
				end
			end
			
	FORCE_CLK_L:
			begin
			time_120us_en <= 1'b1;
			send_data <= {1'b1,parity,write_data};
			if(time_100us_done)
				ps2_data_link <= 1'b1;
			if(time_120us_done)
				begin
				ps2_clk_link <= 1'b0;
				state <= TX;
				end
			else
				ps2_clk_link <= 1'b1;
			end
	TX:
			begin
			time_120us_en <= 1'b0;
			if(ps2_clk_fall)
				time_5us_en <= 1'b1;
			else if(time_5us_done && num < 4'd10)
				begin
				time_5us_en <= 1'b0;
				num <= num + 1'b1;
				ps2_data_link <= ~send_data[0];
				send_data <= {1'b0,send_data[9:1]};
				end	
			else if(num == 4'd10 &&time_5us_done)
				begin
				ps2_data_link <= 1'b0;
				state <= ACK;
				end	
			end	
	ACK: 
			begin
			if(ps2_clk_fall)
				begin
				ack <= ps2_data;
				state <= CHECK;
				end
			end
	CHECK:
			begin
			if(ack)
				begin
				state <= ACK;
				error <= 1'b1;
				end
			else if(ps2_clk_pose)
				begin
				state <= IDLE;
				error <= 1'b0;
				end
			end
	default:state <= IDLE;		
	endcase
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			time_120us_cnt <= 13'b0; 
		else if(~time_120us_en)
			time_120us_cnt <= 13'b0;
		else if(~time_120us_done)
			time_120us_cnt <= time_120us_cnt + 1'b1;
		else
			time_120us_cnt <= 13'b0;
	end
	
	assign time_120us_done = (time_120us_cnt == TIME_120US);
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			time_100us_cnt <= 13'b0; 
		else if(~time_120us_en)
			time_100us_cnt <= 13'b0;
		else if(~time_100us_done)
			time_100us_cnt <= time_100us_cnt + 1'b1;
		else
			time_100us_cnt <= 13'b0;
	end
	
	assign time_100us_done = (time_100us_cnt == TIME_100US);
	
	always @ (posedge clk or negedge rst_n)
	begin
		if(~rst_n)
			time_5us_cnt <= 8'b0; 
		else if(~time_5us_en)
			time_5us_cnt <= 8'b0;
		else if(~time_5us_done)
			time_5us_cnt <= time_5us_cnt + 1'b1;
		else
			time_5us_cnt <= 8'b0;
	end
	
	assign time_5us_done = (time_5us_cnt == TIME_5US);
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		begin
		key_on <= 1'b0;
		relase <= 1'b0;
		end
	else if (num == 4'd10 )
		begin
	 	if(data[7:0] == 8'hf0)
	 		begin
			relase <= 1'b1;
			key_on <=1'b0;
			end
		else if(~relase)
			key_on <= 1'b1;
	 	else 
		begin
			relase <= 1'b0;
			key_on <= 1'b0;
		end	
		end
	end	
	
	always @ (posedge clk or negedge rst_n)																																			
		if(~rst_n)
			begin
		    key_onr <= 1'b0;
		    key_onrr <= 1'b0;
		    end
		else
			begin
			 key_onr <= key_on;
			 key_onrr <= key_onr;
			 end
 
    wire   key = (~key_onrr && key_onr);
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		begin
		data1 <= 8'b0;
		data2 <= 8'b0;
		data3 <= 8'b0;
		end
	else if(num == 4'd10)
		begin
		data1 <= data[7:0];
		data2 <= data1;
		data3 <= data2;
		end
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		shift_right <= 1'b0;
	else if(data2 == 8'hf0  && data1 == RIGHT_SHIFT)
		shift_right <= 1'b0;
	else if(data1 == RIGHT_SHIFT)
		shift_right <= 1'b1;	
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		shift_left <= 1'b0;
	else if(data2 == 8'hf0  && data1 == LEFT_SHIFT)
		shift_left <= 1'b0;
	else if(data1 == LEFT_SHIFT )
		shift_left <= 1'b1;
	end
	
	assign shift = (shift_left || shift_right);
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		capslock_key <= 1'b0;
	else if(data2 == 8'hf0  && data1 == CAPS )
		capslock_key <= 1'b0;	
	else if(data1 == CAPS)  
		capslock_key <= 1'b1;
	
	end
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		capslock_r <= 1'b0;
	else
		capslock_r <= capslock_key;
	end
	
	assign caps_sw = (~capslock_r && capslock_key);
	
	always @ (posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		capslock <= 1'b0;
	else if(caps_sw)
		capslock <= ~capslock;
	end
	
	always @(posedge clk or negedge rst_n)
	begin
	if(~rst_n)
		key_value <= 8'b0;
	else if(state == WAIT&&num == 4'd10 && data[7:0] !=8'hf0 &&key_on)
		 key_value <= data[7:0];
	end
	

  endmodule