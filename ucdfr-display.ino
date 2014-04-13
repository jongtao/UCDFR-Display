#include <Arduino.h>
#include "lcd.h"
#include "graphics.h"



void setup()
{
	Serial.begin(9600);

	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();
} // setup()




void loop()
{
	
	unsigned char lcdBuffer[2][8][64];
	unsigned char i, j, k;

	LcdInputs Inputs;
	Inputs.di = Inputs.rw = 0;
	Inputs.cs_one = Inputs.cs_two = 1;

	
	// Test Black
	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x0F;

	lcd_draw(lcdBuffer);
	delay(1000);

	// Test White
	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x00;

	lcd_draw(lcdBuffer);
	delay(1000);

	lcd_clear();



// Drawing Text routine
/*
int x, y, page;
x=0;
y=0;
y=y/8;

	for(j=0; j<9; j++)
	{
		if(j>8)
			page = y + 1;
		else
			page = y;

		for(i=0; i<8; i++)
			lcdBuffer[1][0][i + x] ^= !!(A[j] & (0x01<< 7-i)) << j;
	}
*/
	graphics_blit_char(lcdBuffer, 0, 0, A);
	graphics_blit_char(lcdBuffer, 10, 5, A);
	lcd_draw(lcdBuffer);

delay(3000);



for(;;);


// Test Increments
for(;;)
	for(unsigned char i=0; i<8; i++)
	{
		Inputs.di = 0;
		Inputs.db = PAGE_COMMAND + i;
		lcd_input(&Inputs); // set page = 1

		Inputs.di = 1;
		Inputs.db= 0xFF;
		lcd_input(&Inputs); // write data
		//delay(200);
	} // for i
} // loop()

