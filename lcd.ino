#include <lcd.h>


void lcd_db_shift(unsigned char value)
{
	for(unsigned char i=0; i<8; i++)
	{
		PORTB &= ~(1 << 1);
		PORTB |= ((1 & value) << 1);	// Push bit to DB_DATA
		PORTB &= ~(1 << 2);						// DB_CLK 0
		PORTB |= (1 << 2);						// DB_CLK 1
		value >>= 1;
	} // for i

	PORTB &= ~(1 << 2);							// DB_CLK 0
} // lcd_clearDB()



void lcd_input(LcdInputs* Inputs)
{
	PORTB &= ~(1);								// E 0

	PORTD	&= ~(1 << 6);
	PORTD |= (Inputs->di << 6);		// DI di
	PORTD	&= ~(1 << 7);
	PORTD |= (Inputs->rw << 7);		// RW rw

	lcd_db_shift(Inputs->db);

	PORTB &= ~(1 << 3);
	PORTB |= (Inputs->cs_one << 3);	// CS_ONE cs_one
	PORTB &= ~(1 << 4);
	PORTB |= (Inputs->cs_two << 4); // CS_TWO cs_two

	PORTB |= (1);		// E 1
	PORTB &= ~(1);	// E 0
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
	DDRD = 0xC0; // enable pins 6-7
	DDRB = 0x3F; // enable pins 8-13
	PORTB |= (1 << 5); // RST 1
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

	for(unsigned char page=0; page<8; page++) // Begin drawing
	{
		lcd_input(&Page);
		Page.db++;

		for(unsigned char cs=0; cs<2; cs++)
		{
			Inputs.cs_one = cs;
			Inputs.cs_two = !cs;

			for(unsigned char column=0; column<64; column++)
			{
				Inputs.db = lcdBuffer[cs][page][column];
				lcd_input(&Inputs);
				//delay(10);
			} // for column
		} // for cs
	} // for page
} // lcd_draw()


