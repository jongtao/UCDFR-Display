#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
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
	unsigned char di, rw, db, cs_one, cs_two;
} LcdInputs;


void lcd_db_shift(unsigned char value);

void lcd_input(LcdInputs* Inputs);

void lcd_clear();

void lcd_init();

void lcd_onoff();

void lcd_draw(unsigned char lcdBuffer[2][8][62]);

#endif
