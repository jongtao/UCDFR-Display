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

	LcdInputs Inputs;

	Inputs.di = Inputs.rw = 0;
	Inputs.cs_one = Inputs.cs_two = 1;


	for(unsigned char k=0; k<2; k++)
		for(unsigned char i=0; i<8; i++)
			for(unsigned char j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0xFF;

	lcd_draw(lcdBuffer);

	delay(1000);

	for(unsigned char k=0; k<2; k++)
		for(unsigned char i=0; i<8; i++)
			for(unsigned char j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x00;

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
		//delay(200);
	} // for i
} // loop()

