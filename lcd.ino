#include <lcd.h>


void lcd_db_shift(unsigned short value)
{
	for(short i=0; i<8; i++)
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

	Page.db = 0xB8;
	Page.di = 0;
	Page.rw = 0;
	Page.cs_one = 1;
	Page.cs_two = 1;

	Blank.db = 0x00;
	Blank.di = 1;
	Blank.rw = 0;
	Blank.cs_one = 1;
	Blank.cs_two = 1;

	for(short i=0; i<8; i++)
	{
		lcd_input(&Page);
		Page.db++;

		for(short j=0; j<128; j++)
			lcd_input(&Blank);
	} //for i
} // lcd_clear()

/*
void lcd_set_page(unsigned short page)
{
	LCDInputs Page;

	Page.db = 0xB8 + page;
	Page.di = 0;
	Page.rw = 0;
	Page.cs_one = 1;
	Page.cs_two = 1;

	lcd_input(&Page);
} // lcd_set_page()



void lcd_set_address(unsigned short address)
{
	LCDInputs Address;

	Address.db = 0x70 & address;
	Address.di = 0;
	Address.rw = 0;
	Address.cs_one = 1;
	Address.cs_two = 1;

} // lcd_set_address()
*/

