
module ps2_lcd1602
(
	CLK_20M,RST_N,PS2_CLK,PS2_DATA,
	BEEP,
	//LCD_1602
	LCD_DATA,   //8-bit LCD DATA
	LCD_RW,     //LCD Read/Write Select,0=Write,1=Read
   LCD_EN,     //LCD Enable
	LCD_RS      //LCD Command/Data select,0=Command,1=Data
);
//LCD_1602
output [7:0] LCD_DATA; //8-bit LCD DATA
output LCD_RW;   //LCD Read/Write Select,0=Write,1=Read
output LCD_EN;   //LCD Enable
output LCD_RS;   //LCD Command/Data select,0=Command,1=Data
//---------------------------------------------------------------------------
//--	
//---------------------------------------------------------------------------
input				CLK_20M;				
input				RST_N;				
input				PS2_CLK;				
input				PS2_DATA;			
output 			BEEP;					

//---------------------------------------------------------------------------
//--	
//---------------------------------------------------------------------------
wire	 [15:0]	o_ps2_data;			
//---------------------------------------------------------------------------
//--
//---------------------------------------------------------------------------

Ps2_Module			Ps2_Init
(
	.CLK_20M		(CLK_20M	),	
	.RST_N			(RST_N		),	
	.PS2_CLK		(PS2_CLK	),	
	.PS2_DATA		(PS2_DATA	),	
	.o_ps2_data		(o_ps2_data	)	
);

LCD_1602          LCD_1602_Init
(
   .reset         (RST_N   	),
   .clock   	  (CLK_20M	),
   .LCD_DATA      (LCD_DATA ),
	.PS2_LCD_DATA (o_ps2_data),
   .LCD_RW        (LCD_RW    ),
   .LCD_EN		  (LCD_EN    ),
   .LCD_RS		  (LCD_RS    )
);
 
Beep_Module			Beep_Init
(
	.CLK_20M		(CLK_20M	),	
	.RST_N			(RST_N		),	
	.BEEP			(BEEP		),	
	.KEY			(o_ps2_data	)
);

endmodule
