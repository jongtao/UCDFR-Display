#include <Arduino.h>
#include <avr/progmem.h>

#include "lcd.h"
#include "graphics.h"


void setup()
{
	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();
} // setup()



void loop()
{
	
	uint8_t lcdBuffer[2][8][64];
	uint8_t num;
	char string[8];

	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46,
		"Temp: 40C (ok)\nTorque Bias: 60%");
	lcd_draw(lcdBuffer);
	delay(3000);
/*
	for(;;)
	{
		get_inputs();
		process_data();
		put_outputs();
		draw();
	} // run loop
*/




		for(num = 0; num < 5; num++)
		{
			graphics_clear_buffer(lcdBuffer);
			graphics_print(lcdBuffer, 0, 0,
				"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill ");
			lcd_draw(lcdBuffer);
			delay(500);

			graphics_clear_buffer(lcdBuffer);
			graphics_print(lcdBuffer, 0, 0,
				"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill_");
			lcd_draw(lcdBuffer);
			delay(500);
		}

	graphics_clear_buffer(lcdBuffer);
	lcd_draw(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapBigNum, 0, 0, 128, 40, 456);
	lcd_draw(lcdBuffer);
	delay(3000);
	
	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapCharacters, 0, 0, 128, 8, 480);
	lcd_draw(lcdBuffer);
	delay(3000);

	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, bitmapSae, 0, 0, 128, 57, 128);
	lcd_draw(lcdBuffer);
	delay(3000);

for(char asdf=0;asdf<5;asdf++)
	for(num = 0; num <= 100; num++)
	{
		graphics_clear_buffer(lcdBuffer);
		sprintf(string, "%3d%%", num);
		graphics_num(lcdBuffer, 0, 0, string);
		graphics_print(lcdBuffer, 16, 46,
			"BATTERY CRITICAL!");
		lcd_draw(lcdBuffer);

	}

} // loop()




