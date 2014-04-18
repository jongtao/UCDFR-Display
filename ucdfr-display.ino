#include <Arduino.h>
#include "lcd.h"
#include "graphics.h"
#include <avr/progmem.h>



void setup()
{
	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();
} // setup()




void loop()
{
	uint8_t lcdBuffer[2][8][64], i, j, k;
	unsigned long time;

	LcdInputs Inputs;
	Inputs.di = Inputs.rw = 0;
	Inputs.cs_one = Inputs.cs_two = 1;

/* Test buffer
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

/* Random character
	for(;;)
	{
		graphics_blit_char(lcdBuffer, random(122), random(57), A);
		lcd_draw(lcdBuffer);
	}
*/
uint8_t num;
for(;;)
{
for(num = 0; num < 8; num++)
{
	graphics_clear_buffer(lcdBuffer);
	graphics_print(lcdBuffer, 0, 0,
		"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill ");
	lcd_draw(lcdBuffer);
	delay(1000);

	graphics_clear_buffer(lcdBuffer);
	graphics_print(lcdBuffer, 0, 0,
		"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill_");
	lcd_draw(lcdBuffer);
	delay(1000);
}

	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapBigNum, 0, 0, 128, 56, 408);
	lcd_draw(lcdBuffer);
	delay(5000);
	
	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapCharacters, 0, 0, 128, 8, 480);
	lcd_draw(lcdBuffer);
	delay(5000);

	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapSae, 0, 0, 128, 57, 128);
	lcd_draw(lcdBuffer);
	delay(5000);
}

	for(;;); // stop

	delay(3000);

} // loop()

