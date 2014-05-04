#include "lcd.h"

/*

void lcd_db_shift(uint8_t value)
{
	uint8_t i;
	for(i=0; i<8; i++)
	{
		PORTB &= ~(1 << 1);
		PORTB |= ((1 & value) << 1);	// Push bit to DB_DATA
		PORTB &= ~(1 << 2);						// DB_CLK 0
		PORTB |= (1 << 2);						// DB_CLK 1
		value >>= 1;
	} // for i

	PORTB &= ~(1 << 2);							// DB_CLK 0
} // lcd_clearDB()
*/


void lcd_input(LcdInputs* Inputs)
{
	uint8_t temp_db = Inputs->db;

	PORTF &= ~(1 << 2);							// E 0

	PORTF	&= ~(1 << 4);
	PORTF |= (Inputs->di << 4);			// DI di
	PORTF	&= ~(1 << 3);
	PORTF |= (Inputs->rw << 3);			// RW rw

	PORTF &= 0xFC;									// Clear DB0 to DB1
	PORTE &= 0x3F;									// Clear DB2 to DB3
	PORTB &= 0xF0;									// Clear DB4 to DB7

	PORTF |= ((1 & temp_db) << 1);	// Push bits from DB0 to DB7
	temp_db >>= 1;
	PORTF |= ((1 & temp_db) << 0);
	temp_db >>= 1;

	PORTE |= ((1 & temp_db) << 6);
	temp_db >>= 1;
	PORTE |= ((1 & temp_db) << 7);
	temp_db >>= 1;

	PORTB |= ((1 & temp_db) << 0);
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 1);
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 2);
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 3);
	// End Data

	PORTB &= ~(1 << 4);
	PORTB |= (Inputs->cs_one << 4);	// CS1 CS_one
	PORTB &= ~(1 << 5);
	PORTB |= (Inputs->cs_two << 5); // CS2 cs_two

	PORTF |= (1 << 2);		// E 1
	PORTF &= ~(1 << 2);		// E 0
} // lcd_input()



void lcd_clear()
{
	LcdInputs Page, Blank;
	uint8_t i, j;

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

	for(i=0; i<8; i++)
	{
		lcd_input(&Page);
		Page.db++;

		for(j=0; j<128; j++)
			lcd_input(&Blank);
	} //for i

	Page.db = ADDRESS_COMMAND;
	lcd_input(&Page); // set address to 0
} // lcd_clear()



void lcd_init()
{
	DDRB = 0x7F;				// enable PB0 to PB6
	DDRE = 0xC0;				// enable PE6 to PE7
	DDRF = 0x1F;				// enable PF0 to PF4
	PORTB |= (1 << 6);	// RST 1

	lcd_onoff();
	lcd_clear();
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



void lcd_draw(uint8_t lcdBuffer[2][8][64])
{
	uint8_t page, cs, column;
	LcdInputs Inputs, Page;

	Inputs.db = ADDRESS_COMMAND; // address = 0
	Inputs.di = 0;
	Inputs.rw = 0;
	Inputs.cs_one = 1;
	Inputs.cs_two = 1;
	lcd_input(&Inputs); // Set address to zero
	Inputs.di = 1;

	Page.db = PAGE_COMMAND; // address = 0
	Page.di = 0;
	Page.rw = 0;
	Page.cs_one = 1;
	Page.cs_two = 1;

	for(page=0; page<8; page++) // Begin drawing
	{
		lcd_input(&Page);
		Page.db++;

		for(cs=0; cs<2; cs++)
		{
			Inputs.cs_one = cs;
			Inputs.cs_two = !cs;

			for(column=0; column<64; column++)
			{
				Inputs.db = lcdBuffer[cs][page][column];
				lcd_input(&Inputs);
			} // for column
		} // for cs
	} // for page
} // lcd_draw()


