#include "logic.h"

// SEVEN LINE MAXIMUM

void logic(uint8_t lcdBuffer[2][8][64])
{
	graphics_clear_buffer(lcdBuffer);

	//logic_splash(lcdBuffer);
	logic_menu(lcdBuffer);
	lcd_draw(lcdBuffer);
	_delay_ms(5000);
	for(;;);

	logic_demo(lcdBuffer);
} // put_screen



void logic_splash(uint8_t lcdBuffer[2][8][64])
{
	graphics_print(lcdBuffer, 16, 16, "UC Davis\n\n  Formula Racing");
} // screen_spash()



void logic_menu(uint8_t lcdBuffer[2][8][64])
{

	graphics_print(lcdBuffer, 34, 0, "Info. Select");

	graphics_print(lcdBuffer, 108, 18, "\x7F");

	graphics_print(lcdBuffer, 0, 18, "Top Section    > [ ]");
	graphics_print(lcdBuffer, 0, 27, "Bottom Section > [ ]");
	graphics_print(lcdBuffer, 0, 36, "LED Bar        > [ ]");
}



void logic_demo(uint8_t lcdBuffer[2][8][64])
{
		uint8_t i, j;
		char string[8];

		graphics_clear_buffer(lcdBuffer);
		sprintf(string, "%2d.%d", 64,2);
		graphics_num(lcdBuffer, 0, 0, string);
		graphics_print(lcdBuffer, 105, 20, "mph");
		graphics_print(lcdBuffer, 16, 46,
			"Temp: 40C (ok)\nTorque Bias: 60%");
		lcd_draw(lcdBuffer);
		_delay_ms(3000);

		for(i=0; i< 5; i++)
		{
			graphics_clear_buffer(lcdBuffer);
			graphics_print(lcdBuffer, 0, 0,
				"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill ");
			lcd_draw(lcdBuffer);
			_delay_ms(500);

			graphics_clear_buffer(lcdBuffer);
			graphics_print(lcdBuffer, 0, 0,
				"LIL'CHEW CHEW\nv2.2\nI've not been\nprogrammed for\nRegret <3 <3\n\n$ sudo kill_");
			lcd_draw(lcdBuffer);
			_delay_ms(500);
		} // for i

		graphics_clear_buffer(lcdBuffer);
		lcd_draw(lcdBuffer);
		graphics_blit(lcdBuffer, 0, 0, bitmapBigNum, 0, 0, 128, 40, 456);
		lcd_draw(lcdBuffer);
		_delay_ms(3000);
		
		graphics_clear_buffer(lcdBuffer);
		graphics_blit(lcdBuffer, 0, 0, bitmapCharacters, 0, 0, 128, 8, 480);
		lcd_draw(lcdBuffer);
		_delay_ms(3000);

		graphics_clear_buffer(lcdBuffer);
		graphics_blit(lcdBuffer, 0, 0, bitmapSae, 0, 0, 128, 57, 128);
		lcd_draw(lcdBuffer);
		_delay_ms(3000);

		for(j=0 ;j<5 ;j++)
			for(i=0; i<=100; i++)
			{
				graphics_clear_buffer(lcdBuffer);
				sprintf(string, "%3d%%", i);
				graphics_num(lcdBuffer, 0, 0, string);
				graphics_print(lcdBuffer, 16, 46, "BATTERY CRITICAL!");
				lcd_draw(lcdBuffer);
			} // for i

	} // screen_demo()
