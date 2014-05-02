#ifndef LCD_H
#define LCD_H
#include <avr/io.h>

/*
#define DI      6
#define RW      7
#define E       8
#define DB_DATA 9
#define DB_CLK  10
#define CS_ONE  11
#define CS_TWO  12
#define RST     13
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

