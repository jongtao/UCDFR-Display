#ifndef GRAPHICS_H
#define GRAPHICS_H

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_PAGES 8
#define LCD_CS_WIDTH 64

#include <avr/pgmspace.h>

void graphics_blit(uint8_t lcdBuffer[2][8][64],
	uint8_t dstX, uint8_t dstY, const uint8_t *bitmap,
	uint16_t srcX, uint16_t srcY, uint8_t width,
	uint8_t height, uint16_t rowLen);

void graphics_clear_buffer(uint8_t lcdBuffer[2][8][64]);

void graphics_print(uint8_t lcdBuffer[2][8][64], uint8_t dstX, uint8_t dstY,
	char* string);

void graphics_num(uint8_t lcdBuffer[2][8][64], uint8_t dstX, uint8_t dstY,
	char* string);


extern const uint8_t bitmapCharacters[480];
extern const uint8_t bitmapBigNum[2280];
extern const uint8_t bitmapSae[912];


#endif
