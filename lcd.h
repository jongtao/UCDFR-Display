#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

#define RST 6
#define DI 7
#define RW 8
#define E 9
#define DB_DATA 10
#define DB_CLK 11
#define CS_ONE 12
#define CS_TWO 13

#define ONOFF_COMMAND 0x3F
#define ADDRESS_MASK 0x3F
#define ADDRESS_COMMAND 0x40
#define PAGE_MASK 0x07
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

/*
void lcd_set_page(unsigned char page);

void lcd_set_address(unsigned char address);
*/
#endif

