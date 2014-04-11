#include <Arduino.h>
#include "lcd.h"




void setup()
{
	for(short i=0; i<=13; pinMode(i++, OUTPUT)); // All pins to output mode
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
} // setup()




void loop()
{
	LcdInputs Inputs;

	Inputs.db = 0x3F; // Toggle on/off
	Inputs.di = 0;
	Inputs.rw = 0;
	Inputs.cs_one = 1;
	Inputs.cs_two = 1;

	lcd_input(&Inputs); //LCD ON

	Inputs.db = 0xB8;
	lcd_input(&Inputs); // set page = 1

	Inputs.di = 1;
	Inputs.db= 0x01;
	lcd_input(&Inputs); // write data
	delay(1000);
} // loop()

