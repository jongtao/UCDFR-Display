#ifndef GRAPHICS_H
#define GRAPHICS_H

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_PAGES 8

#include <Arduino.h>

/*
extern typedef struct
{
	unsigned char letters[26];
} Character;
*/

// bitmaps defined as 8 bits across in rows of the array

void graphics_blit_char(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char character[8]);

void graphics_blit(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char *bitmap, unsigned char w,
	unsigned char h);

void graphics_clear_buffer(unsigned char lcdBuffer[2][8][64]);


extern unsigned char A[8] = {0x38, 0x44, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x82};

#endif
