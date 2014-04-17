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
	unsigned long time;

	LcdInputs Inputs;
	Inputs.di = Inputs.rw = 0;
	Inputs.cs_one = Inputs.cs_two = 1;

/*
while(1)
{
	

	// Test Black
	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0xFF;

	lcd_draw(lcdBuffer);
	//delay(1000);

	// Test White
	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x00;

	time = millis();
	lcd_draw(lcdBuffer);

	time = millis() - time;
	Serial.println(time);
}
	delay(1000);

	lcd_clear();
*/
	graphics_clear_buffer(lcdBuffer);

/*
	for(;;)
	{
		graphics_blit_char(lcdBuffer, random(122), random(57), A);
		lcd_draw(lcdBuffer);
	}
*/

	graphics_blit(lcdBuffer, 0, 2, characters, 0, 0, 128, 8, 480);
	lcd_draw(lcdBuffer);

	for(;;); // stop

	delay(3000);



//for(;;);


// Test Increments
	for(;;)
		for(i=0; i<8; i++)
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

