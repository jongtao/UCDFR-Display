#include <lcd.h>


void lcd_db_shift(unsigned char value)
{
	for(unsigned char i=0; i<8; i++)
	{
		digitalWrite(DB_DATA, 0x1 & value);
		digitalWrite(DB_CLK, 0);
		digitalWrite(DB_CLK, 1);
		value >>= 1;
	} // for i

	digitalWrite(DB_CLK, 0);
} // lcd_clearDB()



void lcd_input(LcdInputs* Inputs)
{
		digitalWrite(E, 0);

		digitalWrite(DI, Inputs->di);
		digitalWrite(RW, Inputs->rw);
		lcd_db_shift(Inputs->db);
		digitalWrite(CS_ONE, Inputs->cs_one);
		digitalWrite(CS_TWO, Inputs->cs_two);
		
		digitalWrite(E, 1);
		digitalWrite(E, 0);
} // lcd_input()



void lcd_clear()
{
	LcdInputs Page, Blank;

	Page.db = PAGE_COMMAND;
	Page.di = 0;
	Page.rw = 0;
	Page.cs_one = 1;
	Page.cs_two = 1;

	Blank.db = 0x00;
	Blank.di = 1;
	Blank.rw = 0;
	Blank.cs_one = 1;
	Blank.cs_two = 1;

	for(unsigned char i=0; i<8; i++)
	{
		lcd_input(&Page);
		Page.db++;

		for(unsigned char j=0; j<128; j++)
			lcd_input(&Blank);
	} //for i

	Page.db = ADDRESS_COMMAND;
	lcd_input(&Page); // set address to 0
} // lcd_clear()



void lcd_init()
{
	pinMode(RST, OUTPUT);
	pinMode(DI, OUTPUT);
	pinMode(RW, OUTPUT);
	pinMode(E, OUTPUT);
	pinMode(DB_DATA, OUTPUT);
	pinMode(DB_CLK, OUTPUT);
	pinMode(CS_ONE, OUTPUT);
	pinMode(CS_TWO, OUTPUT);

	digitalWrite(RST, 1);
} // lcd_init()



void lcd_onoff()
{
	LcdInputs Inputs;
	Inputs.db = ONOFF_COMMAND; // Toggle on/off
	Inputs.di = 0;
	Inputs.rw = 0;
	Inputs.cs_one = 1;
	Inputs.cs_two = 1;
	lcd_input(&Inputs);
} // lcd_onoff()



void lcd_draw(unsigned char lcdBuffer[2][8][64])
{
	LcdInputs Inputs;

	Inputs.db = ADDRESS_COMMAND; // address = 0
	Inputs.di = 0;
	Inputs.rw = 0;
	Inputs.cs_one = 1;
	Inputs.cs_two = 1;
	lcd_input(&Inputs); // Set address to zero

	// write
	for(unsigned char page=0; page<8; page++)
	{
		Inputs.di = 0;
		Inputs.db = PAGE_COMMAND + page;
		Inputs.cs_one = 1;
		Inputs.cs_two = 1;
		lcd_input(&Inputs);
		Inputs.di = 1;

		for(unsigned char cs=0; cs<2; cs++)
		{
			if(cs)
			{
				Inputs.cs_one = 1;
				Inputs.cs_two = 0;
			}
			else
			{
				Inputs.cs_one = 0;
				Inputs.cs_two = 1;
			}// if cs

			for(unsigned char column=0; column<64; column++)
			{
				Inputs.db = lcdBuffer[cs][page][column];
				lcd_input(&Inputs);
				//delay(10);
			} // for column
		} // for cs
	} // for page


} // lcd_draw()


