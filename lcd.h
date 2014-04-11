#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

#define DI 7
#define RW 8
#define E 9
#define DB_DATA 10
#define DB_CLK 11
#define CS_ONE 12
#define CS_TWO 13


typedef struct
{
	short di, rw, cs_one, cs_two;
	unsigned short db;
} LcdInputs;

void lcd_db_shift(unsigned short value);

void lcd_input(LcdInputs* Inputs);

void lcd_clear();

/*
void lcd_set_page(unsigned short page);

void lcd_set_address(unsigned short address);
*/
#endif

