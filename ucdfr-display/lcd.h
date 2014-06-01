#ifndef LCD_H
#define LCD_H
#include <avr/io.h>

/* LCD pins
DI	PB6
RW	PB5
E		PB4
DB0	PB3
DB1	PB2
DB2 PB1
DB3 PB0
DB4 PE7
DB5	PE6
DB6 PF0
DB7	PF1
CS1	PF2
CS2	PF3
RST	PF4
*/


#define ONOFF_COMMAND 0x3F
#define ADDRESS_COMMAND 0x40
#define PAGE_COMMAND 0xB8



typedef struct
{
	uint8_t di, rw, db, cs_one, cs_two;
} LcdInputs;



void lcd_input(LcdInputs* Inputs);
void lcd_clear();
void lcd_init();
void lcd_onoff();
void lcd_draw(uint8_t lcdBuffer[2][8][64]);

#endif

