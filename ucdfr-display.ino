#include <Arduino.h>
#include "lcd.h"
#include "graphics.h"




void setup()
{
	lcd_init();
	lcd_db_shift(0); // Clear DB 8 bit shift register
	lcd_clear();
} // setup()




void loop()
{
	
	unsigned char lcdBuffer[2][8][64];
	LcdInputs Inputs;

	Inputs.di = 0;
	Inputs.rw = 0;
	Inputs.cs_one = 1;
	Inputs.cs_two = 1;

	lcd_onoff();

	unsigned char i, j;
	for(i=0; i<8; i++)
		for(j=0; j<64; j++)
			lcdBuffer[0][i][j] = 0xFF;

	lcd_draw(lcdBuffer);
	delay(3000);

	//for(;;);
	lcd_clear();

for(;;)
	for(unsigned char i=0; i<8; i++)
	{
		Inputs.di = 0;
		Inputs.db = PAGE_COMMAND + i;
		lcd_input(&Inputs); // set page = 1

		Inputs.di = 1;
		Inputs.db= 0xFF;
		lcd_input(&Inputs); // write data
		delay(200);
	} // for i
} // loop()

