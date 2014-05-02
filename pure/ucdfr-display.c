#include <Arduino.h>
#include "lcd.h"
#include "logic.h"



typedef struct
{
	uint8_t screen;
} Datum;


void get_inputs(){}



void put_outputs(){}



void main()
{
	uint8_t running = 1;
	uint8_t lcdBuffer[2][8][64];

	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
	lcd_onoff();

	while(running)
	{
		get_inputs();
		put_outputs();
		logic(lcdBuffer);
		lcd_draw(lcdBuffer);
	} // while running
} // setup()



void loop(){} // loop()

