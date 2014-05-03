#ifndef LCD_H
#define LCD_H
#include <avr/io.h>

/*
DI	PF4
RW	PF3
E		PF2
DB0	PF1
DB1	PF0
DB2 PE6
DB3 PE7
DB4 PB0
DB5	PB1
DB6 PB2
DB7	PB3
CS1	PB4
CS2	PB5
RST	PB6
*/


#define ONOFF_COMMAND 0x3F
#define ADDRESS_COMMAND 0x40
#define PAGE_COMMAND 0xB8



typedef struct
{
	uint8_t di, rw, db, cs_one, cs_two;
} LcdInputs;


void lcd_db_shift(uint8_t value);

void lcd_input(LcdInputs* Inputs);

void lcd_clear();

void lcd_init();

void lcd_onoff();

void lcd_draw(uint8_t lcdBuffer[2][8][64]);

#endif

