#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>

/*
extern typedef struct
{
	unsigned char letters[26];
} Character;
*/

// bitmaps defined as 8 bits across in rows of the array

void graphics_blit_char(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char character[9]);

extern unsigned char A[9] = {0x38, 0x44, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x82};

#endif
