#include "lcd.h"



void lcd_input(LcdInputs* Inputs)
{
	uint8_t temp_db = Inputs->db;

	PORTB &= ~(1 << 4);							// E 0

	PORTB	&= ~(1 << 6);
	PORTB |= (Inputs->di << 6);			// DI di
	PORTB	&= ~(1 << 5);
	PORTB |= (Inputs->rw << 5);			// RW rw

	PORTB &= 0xF0;									// Clear DB0 to DB3
	PORTE &= 0x3F;									// Clear DB4 to DB5
	PORTF &= 0xFC;									// Clear DB6 to DB7

	PORTB |= ((1 & temp_db) << 3);	// Push bits from DB0 to DB7
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 2);	// DB1
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 1);	// DB2
	temp_db >>= 1;
	PORTB |= ((1 & temp_db) << 0);	// DB3
	temp_db >>= 1;
	PORTE |= ((1 & temp_db) << 7);	// DB4
	temp_db >>= 1;
	PORTE |= ((1 & temp_db) << 6);	// DB5
	temp_db >>= 1;
	PORTF |= ((1 & temp_db) << 0);	// DB6
	temp_db >>= 1;
	PORTF |= ((1 & temp_db) << 1);	// DB7
	// End Data

	PORTF &= ~(1 << 2);
	PORTF |= (Inputs->cs_one << 2);	// CS1 CS_one
	PORTF &= ~(1 << 3);
	PORTF |= (Inputs->cs_two << 3); // CS2 cs_two

	PORTB |= (1 << 4);		// E 1
	PORTB &= ~(1 << 4);		// E 0
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
	//PORTB |= (1 << 6);	// RST 1
	PORTF |= (1 << 4);	// RST 1

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


