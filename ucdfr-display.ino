#include <Arduino.h>
#include "lcd.h"
#include "graphics.h"
#include <avr/eeprom.h>



void setup()
{
	//Serial.begin(9600);
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

/* Blit character
	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 2, characters, 0, 0, 128, 8, 480);
	//lcd_draw(lcdBuffer);
*/

	graphics_clear_buffer(lcdBuffer);
	graphics_blit(lcdBuffer, 0, 0, sae, 0, 0, 128, 57, 128);
	delay(1);
	lcd_draw(lcdBuffer);
	for(;;); // stop

	delay(3000);

} // loop()

