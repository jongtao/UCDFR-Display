#include <stdint.h>
#include <stdio.h>

#include "lcd.h"
#include "graphics.h"


void get_inputs(){return;}
void process_data(){return;}
void put_outputs(){return;}
void draw(){return;}



void run_loop()
{
	uint8_t lcdBuffer[2][8][64];

	char string[8];
	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46,
		"Temp: 40C (ok)\nTorque Bias: 60%");
	lcd_draw(lcdBuffer);

	for(;;)
	{
		get_inputs();
		process_data();
		put_outputs();
		draw();
	} // run loop



/* DEMO
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
*/
} // loop()



void start()
{
	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();
} // setup()



int main()
{
	lcd_init();
	//lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();

	uint8_t lcdBuffer[2][8][64];

	char string[8];
	graphics_clear_buffer(lcdBuffer);
	sprintf(string, "%2d.%d", 64,2);
	graphics_num(lcdBuffer, 0, 0, string);
	graphics_print(lcdBuffer, 105, 20, "mph");
	graphics_print(lcdBuffer, 16, 46,
		"Temp: 40C (ok)\nTorque Bias: 60%");
	lcd_draw(lcdBuffer);

	for(;;)
	{
		get_inputs();
		process_data();
		put_outputs();
		draw();
	} // run loop





	return 0;
} // main()

