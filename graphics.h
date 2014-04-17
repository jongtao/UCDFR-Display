#ifndef GRAPHICS_H
#define GRAPHICS_H

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_PAGES 8
#define LCD_CS_WIDTH 64

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
	unsigned char dstX, unsigned char dstY, unsigned char *bitmap,
	unsigned char srcX, unsigned char srcY, unsigned char width,
	unsigned char height, unsigned int rowLen);

void graphics_clear_buffer(unsigned char lcdBuffer[2][8][64]);


extern unsigned char A[8] = {0x38, 0x44, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x82};


extern unsigned char characters[480] =
{

	0x01, 0x14, 0xA2, 0x61, 0x04, 0x12, 0x00, 0x00, 0x00, 0x00, 0x71, 0x1C, 0xE1, 0x7C, 0xDF, 0x73, 0x80, 0x01, 0x02, 0x0E, 0x73, 0xBC, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x14, 0xA7, 0xE6, 0x84, 0x21, 0x14, 0x40, 0x00, 0x01, 0x8B, 0x23, 0x13, 0x41, 0x01, 0x8C, 0x58, 0xC2, 0x01, 0x11, 0x8C, 0x63, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFA, 0x0A, 0x80, 0x40, 0x88, 0x40, 0x00, 0x02, 0x99, 0x02, 0x15, 0x7A, 0x02, 0x8C, 0x58, 0xC4, 0x7C, 0x81, 0xAC, 0x63, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xA7, 0x11, 0x00, 0x40, 0xBF, 0xF0, 0x7C, 0x04, 0xA9, 0x0C, 0xE9, 0x07, 0xC4, 0x73, 0xC0, 0x08, 0x00, 0x42, 0xBF, 0xFD, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xF2, 0xA2, 0xA0, 0x40, 0x88, 0x46, 0x00, 0x08, 0xC9, 0x10, 0x1F, 0x86, 0x28, 0x88, 0x58, 0xC4, 0x7C, 0x84, 0xA4, 0x63, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAF, 0x4E, 0x40, 0x21, 0x14, 0x46, 0x01, 0x90, 0x89, 0x21, 0x11, 0x46, 0x28, 0x88, 0x98, 0xC2, 0x01, 0x00, 0x84, 0x63, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xA2, 0x0D, 0xA0, 0x12, 0x00, 0x02, 0x01, 0x80, 0x73, 0xBE, 0xE1, 0x39, 0xC8, 0x73, 0x00, 0x41, 0x02, 0x04, 0x7C, 0x7C, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif
