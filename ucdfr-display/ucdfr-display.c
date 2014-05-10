#include <stdint.h>
#include <stdio.h>

#include "lcd.h"
#include "graphics.h"
#include "engine.h"
#include "utilities.h"

#include <util/atomic.h>
/*
// DEMO
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
	graphics_blit(lcdBuffer, 0, 0, bitmapBigNum, 0, 0, 128, 40, 456);
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

} // loop()
*/

//volatile Inputs inputs;
int main()
{
	uint8_t lcdBuffer[2][8][64];
	Inputs inputs;
	unsigned long milliseconds_since = 0;
	unsigned long milliseconds_current = 0;

	lcd_init();
	engine_init();
	utilities_init();
	unsigned long last_rev = 0;

	for(;;)
	{


		engine_get_inputs(inputs);
		engine_process_data();
		engine_put_outputs();

 		milliseconds_current = millis();
		if(milliseconds_current - milliseconds_since > 100)
		{
			
			engine_graphics(lcdBuffer);
			lcd_draw(lcdBuffer);
			milliseconds_since = milliseconds_current;

		}
	} // run loop

	return 0;
} // main()

