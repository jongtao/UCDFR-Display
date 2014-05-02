#include "graphics.h"


// CAUTION: beware for uint8_t range
void graphics_blit(uint8_t lcdBuffer[2][8][64],
	uint8_t dstX, uint8_t dstY, const uint8_t *bitmap,
	uint16_t srcX, uint16_t srcY, uint8_t width,
	uint8_t height, uint16_t rowLen)
{
	uint16_t page1, page2, cs, horizontal, i, j, k;
	uint16_t indX, indY;

	//rowLen = (rowLen- 1)/8 + 1;					// convert pixel length to elements (ceil)
	rowLen = rowLen/8;					// convert pixel length to elements (ceil)
	page1 = dstY / LCD_PAGES; 							// find page to start
	k = dstY - LCD_PAGES * page1; 					// offset after page

	for(j=0; j<height; j++)
	{
		page2 = (dstY + j) / LCD_PAGES; 			// compute new page

		if(page2 >= LCD_PAGES) 								// crop bottom if too low
			return;

		if(page1 != page2)
		{
			page1 = page2; 											// increment page
			k=0;
		} // if different page

		indY = (j + srcY)*rowLen;							// Compute Y index in terms of bitmap location

		for(i=0; i<width; i++)
		{
			horizontal = i + dstX;							// horizontal location of pixel being drawn
			cs = 1;
			if(horizontal >= LCD_WIDTH) break;	// stop if out of screen

			if(horizontal >= LCD_CS_WIDTH)
			{
				cs = 0;
				horizontal -= LCD_CS_WIDTH;
			} // if need to move to next CS

			indX = (i + srcX)/8; 								// Compute X index in terms of bitmap location
		
			lcdBuffer[cs][page2][horizontal] |=
				(!!(pgm_read_byte_near(bitmap + indY + indX) &
				(0x01 << (7 - (i + srcX)%8)))) << k; // BLIT
		} // for horizontal pixel in char

		k++;																	// counts bits from top of page
	}// for j
} // graphics_blit_char()




void graphics_clear_buffer(uint8_t lcdBuffer[2][8][64])
{
	uint8_t i, j, k;

	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x00;
} // graphics_clear_buffer()



void graphics_print(uint8_t lcdBuffer[2][8][64], uint8_t dstX, uint8_t dstY,
	char* string)
{
	uint16_t i = 0;
	uint8_t cursor = dstX;

	while(string[i])
	{
		if(string[i] == '\n')
		{
			dstY += 9;
			cursor = dstX;
		} // newline
		else
		{
			if((cursor + 5) > 128)
			{
				dstY += 9;
				cursor = dstX;
			}

			graphics_blit(lcdBuffer, cursor, dstY, bitmapCharacters,
				(string[i] - ' ') * 5, 0, 5, 8, 480);
			cursor += 6;
		} // else

		i++;
	} // until '\0'
} // graphics_print()



void graphics_num(uint8_t lcdBuffer[2][8][64], uint8_t dstX, uint8_t dstY,
	char* string)
{
	uint8_t i = 0;
	uint8_t cursor = dstX;
	uint8_t character;

	while(string[i])
	{
		if('0' <= string[i] && string[i] <= '9')
		{
			graphics_blit(lcdBuffer, cursor, dstY, bitmapBigNum,
				(string[i] - '0') * 32, 0, 32, 40, 456);
			cursor += 30;
		} // if number
		else
			if(string[i] == '.')
			{
				graphics_blit(lcdBuffer, cursor - 14, dstY, bitmapBigNum,
					320, 0, 32, 40, 456);
				cursor += 4;
			} // if decimal place
			else
				if(string[i] == '-')
				{
					graphics_blit(lcdBuffer, cursor, dstY, bitmapBigNum,
						352, 0, 32, 40, 456);
					cursor += 30;
				} // if negative
				else
					if(string[i] == '%')
					{

						graphics_blit(lcdBuffer, cursor, dstY, bitmapBigNum,
							384, 0, 32, 40, 456);
						cursor += 30;
					} // if percent
					else
						if(string[i] == '!')
						{

							graphics_blit(lcdBuffer, cursor, dstY, bitmapBigNum,
								416, 0, 32, 40, 456);
								cursor += 30;
						} // if percent
						else
							if(string[i] == ' ')
								cursor += 30; // space
		i++;
	} // while string
} // graphics_num()





/* BITMAPS */

// RowLength = 480
// width = 5
// height 8
PROGMEM const uint8_t bitmapCharacters[480] =
{
0x01, 0x14, 0xA2, 0x61, 0x04, 0x12, 0x00, 0x00, 0x00, 0x00, 0x71, 0x1C, 0xE1, 0x7C, 0xDF, 0x73, 0x80, 0x01, 0x02, 0x0E, 0x73, 0xBC, 0xEE, 0x7F, 0xEE, 0x8B, 0x83, 0x18, 0x46, 0x2E, 0xF3, 0xBC, 0xEF, 0xC6, 0x31, 0x8C, 0x7E, 0xE0, 0x38, 0x80, 0x40, 0x20, 0x00, 0x80, 0x40, 0x81, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x62, 0x30, 0x15
, 0x01, 0x14, 0xA7, 0xE6, 0x84, 0x21, 0x14, 0x40, 0x00, 0x01, 0x8B, 0x23, 0x13, 0x41, 0x01, 0x8C, 0x58, 0xC2, 0x01, 0x11, 0x8C, 0x63, 0x19, 0x42, 0x11, 0x89, 0x03, 0x28, 0x6E, 0x31, 0x8C, 0x63, 0x12, 0x46, 0x31, 0x8C, 0x42, 0x88, 0x09, 0x40, 0x40, 0x20, 0x00, 0x80, 0xA0, 0x80, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x82, 0x08, 0x0A
, 0x01, 0x01, 0xFA, 0x0A, 0x80, 0x40, 0x88, 0x40, 0x00, 0x02, 0x99, 0x02, 0x15, 0x7A, 0x02, 0x8C, 0x58, 0xC4, 0x7C, 0x81, 0xAC, 0x63, 0x08, 0xC2, 0x10, 0x89, 0x03, 0x48, 0x57, 0x31, 0x8C, 0x63, 0x02, 0x46, 0x31, 0x52, 0x84, 0x84, 0x0A, 0x20, 0x23, 0xBC, 0xF7, 0xB8, 0x8E, 0xF3, 0x0D, 0x22, 0x6B, 0xCE, 0xF3, 0xEE, 0xFE, 0x46, 0x31, 0x8C, 0x7E, 0x82, 0x09, 0x15
, 0x01, 0x00, 0xA7, 0x11, 0x00, 0x40, 0xBF, 0xF0, 0x7C, 0x04, 0xA9, 0x0C, 0xE9, 0x07, 0xC4, 0x73, 0xC0, 0x08, 0x00, 0x42, 0xBF, 0xFD, 0x08, 0xFB, 0xD0, 0xF9, 0x03, 0x88, 0x56, 0xB1, 0xF4, 0x7C, 0xE2, 0x45, 0x55, 0x21, 0x08, 0x82, 0x08, 0x00, 0x00, 0x63, 0x08, 0xC5, 0xD1, 0x89, 0x05, 0x42, 0x56, 0x31, 0x8C, 0x71, 0x04, 0x46, 0x31, 0x54, 0x45, 0x02, 0x06, 0xAA
, 0x01, 0x01, 0xF2, 0xA2, 0xA0, 0x40, 0x88, 0x46, 0x00, 0x08, 0xC9, 0x10, 0x1F, 0x86, 0x28, 0x88, 0x58, 0xC4, 0x7C, 0x84, 0xA4, 0x63, 0x08, 0xC2, 0x13, 0x89, 0x23, 0x48, 0x46, 0x71, 0x85, 0x68, 0x12, 0x45, 0x55, 0x51, 0x10, 0x81, 0x08, 0x00, 0x03, 0xE3, 0x08, 0xFC, 0x91, 0x89, 0x05, 0x82, 0x56, 0x31, 0x8C, 0x60, 0xE4, 0x46, 0x35, 0x24, 0x48, 0x82, 0x08, 0x55
, 0x00, 0x00, 0xAF, 0x4E, 0x40, 0x21, 0x14, 0x46, 0x01, 0x90, 0x89, 0x21, 0x11, 0x46, 0x28, 0x88, 0x98, 0xC2, 0x01, 0x00, 0x84, 0x63, 0x19, 0x42, 0x11, 0x89, 0x23, 0x28, 0x46, 0x31, 0x84, 0xA5, 0x12, 0x44, 0x9B, 0x89, 0x20, 0x80, 0x88, 0x00, 0x04, 0x63, 0x08, 0xC0, 0x8F, 0x89, 0x05, 0x42, 0x56, 0x31, 0xF3, 0xE0, 0x15, 0x45, 0x55, 0x53, 0xD0, 0x82, 0x08, 0x0A
, 0x01, 0x00, 0xA2, 0x0D, 0xA0, 0x12, 0x00, 0x02, 0x01, 0x80, 0x73, 0xBE, 0xE1, 0x39, 0xC8, 0x73, 0x00, 0x41, 0x02, 0x04, 0x7C, 0x7C, 0xEE, 0x7E, 0x0F, 0x8B, 0x9D, 0x1F, 0xC6, 0x2E, 0x83, 0x62, 0xE2, 0x38, 0x91, 0x89, 0x3E, 0xE0, 0x38, 0x1F, 0x03, 0xFC, 0xF7, 0xB8, 0x81, 0x8B, 0xA5, 0x27, 0x46, 0x2E, 0x80, 0x61, 0xE2, 0x3C, 0x8A, 0x88, 0x7E, 0x62, 0x30, 0x15
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00
};



// RowLength = 456 
// width = 32
// height = 40
PROGMEM const uint8_t bitmapBigNum[2280] = 
{

0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x01, 0xFF, 0xF8, 0x00, 0x03, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x7F, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x07, 0xFF, 0x80, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x00, 0x7F, 0xFE, 0x00, 0x01, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFE, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x3F, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x00, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0x80, 0x0F, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x01, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x01, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x03, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x03, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x07, 0xFC, 0x3F, 0xE0, 0x03, 0xFF, 0xF8, 0x00, 0x0F, 0x80, 0x3F, 0xE0, 0x0E, 0x00, 0x3F, 0xF0, 0x00, 0x07, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x03, 0xFC, 0x00, 0xE0, 0x1F, 0xFF, 0xFF, 0xE0, 0x0F, 0xF8, 0x3F, 0xC0, 0x0F, 0xF0, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x1F, 0x80, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x07, 0xF8, 0x1F, 0xE0, 0x03, 0xE7, 0xF8, 0x00, 0x0C, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x0F, 0xFF, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x20, 0x00, 0x00, 0x1F, 0xC0, 0x0F, 0xF0, 0x1F, 0xE0, 0x0F, 0xE0, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x0F, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x02, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xFF, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xE0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x07, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x1F, 0xBF, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x07, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x1F, 0xBF, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x07, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x3F, 0x3F, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x0F, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x7F, 0x3F, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE0, 0x0F, 0xE0, 0x1F, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x1F, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x7E, 0x3F, 0xC0, 0x07, 0xE0, 0x00, 0x00, 0x1F, 0xC3, 0xF8, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x0F, 0xF0, 0x1F, 0xC0, 0x1F, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x0C, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x3F, 0xFF, 0xC0, 0x00, 0xFC, 0x3F, 0xC0, 0x07, 0xEF, 0xF0, 0x00, 0x1F, 0xCF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x07, 0xF0, 0x1F, 0xC0, 0x1F, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x3C, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x3F, 0xFF, 0x80, 0x01, 0xFC, 0x3F, 0xC0, 0x07, 0xFF, 0xFC, 0x00, 0x1F, 0xDF, 0xFF, 0x80, 0x00, 0x00, 0xFE, 0x00, 0x07, 0xF8, 0x3F, 0xC0, 0x1F, 0xC0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x01, 0xFE, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE3, 0xC7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x3F, 0xFE, 0x00, 0x01, 0xF8, 0x3F, 0xC0, 0x07, 0xFF, 0xFF, 0x00, 0x1F, 0xFF, 0xFF, 0xC0, 0x00, 0x01, 0xFE, 0x00, 0x03, 0xFF, 0xFF, 0x80, 0x1F, 0xE0, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x07, 0xF0, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE7, 0xE7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x3F, 0xF8, 0x00, 0x03, 0xF0, 0x3F, 0xC0, 0x07, 0xFF, 0xFF, 0x80, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0x01, 0xFC, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x1F, 0xE0, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x3F, 0xC0, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE7, 0xE7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0x00, 0x07, 0xF0, 0x3F, 0xC0, 0x07, 0xFF, 0xFF, 0xC0, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x0F, 0xF0, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE7, 0xE7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x07, 0xE0, 0x3F, 0xC0, 0x07, 0xFF, 0xFF, 0xE0, 0x1F, 0xFC, 0x1F, 0xF0, 0x00, 0x03, 0xFC, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x0F, 0xF8, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE7, 0xE7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x3F, 0xFF, 0xC0, 0x0F, 0xC0, 0x3F, 0xC0, 0x07, 0x80, 0xFF, 0xE0, 0x1F, 0xF8, 0x0F, 0xF0, 0x00, 0x03, 0xF8, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE3, 0xC7, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x0F, 0xC0, 0x3F, 0xC0, 0x04, 0x00, 0x3F, 0xF0, 0x1F, 0xF0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x03, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0x80, 0x3F, 0xC0, 0x00, 0x00, 0x1F, 0xF0, 0x1F, 0xF0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x3F, 0xC0, 0x03, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x01, 0xFC, 0x07, 0xC0, 0x00, 0x07, 0xC0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x1F, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x0F, 0xF8, 0x1F, 0xE0, 0x03, 0xF8, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xF0, 0x1F, 0xE0, 0x01, 0xFF, 0xFB, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x0F, 0xE0, 0x1F, 0xF0, 0x00, 0x07, 0xC0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xE0, 0x03, 0xF8, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xE0, 0x0F, 0xE0, 0x00, 0xFF, 0xF3, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x1F, 0x80, 0x7F, 0xFC, 0x00, 0x07, 0xC0, 0x00, 0x00
, 0x1F, 0xE0, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xE0, 0x03, 0xF8, 0x00, 0x0F, 0xE0, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x3F, 0xC3, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x1E, 0x00, 0x7F, 0xFC, 0x00, 0x07, 0xC0, 0x00, 0x00
, 0x0F, 0xE0, 0x07, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xE0, 0x03, 0xF8, 0x00, 0x1F, 0xE0, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x10, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x0F, 0xE0, 0x03, 0xF8, 0x00, 0x1F, 0xE0, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x0F, 0xE0, 0x03, 0xF8, 0x00, 0x1F, 0xC0, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x07, 0xF8, 0x0F, 0xE0, 0x03, 0xF8, 0x00, 0x3F, 0xC0, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x07, 0xF0, 0x1F, 0xE0, 0x00, 0x07, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x0F, 0xF8, 0x0F, 0xF0, 0x07, 0xF8, 0x00, 0x3F, 0x80, 0x00, 0x1F, 0xC0, 0x07, 0xF0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x07, 0xF8, 0x1F, 0xE0, 0x00, 0x07, 0xF8, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x18, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x0F, 0xF0, 0x0F, 0xF0, 0x07, 0xF0, 0x00, 0x7F, 0x80, 0x00, 0x1F, 0xE0, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x1F, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x07, 0xFC, 0x3F, 0xE0, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0x80, 0x7F, 0xF0, 0x00, 0x00, 0x3F, 0xC0, 0x08, 0x00, 0x3F, 0xF0, 0x07, 0xF8, 0x0F, 0xF0, 0x00, 0x7F, 0x80, 0x00, 0x0F, 0xF0, 0x1F, 0xE0, 0x06, 0x00, 0x3F, 0xE0, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x3F, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x03, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0x00, 0xFF, 0xE0, 0x07, 0xFC, 0x1F, 0xF0, 0x00, 0x7F, 0x00, 0x00, 0x0F, 0xF8, 0x3F, 0xE0, 0x07, 0xC0, 0xFF, 0xC0, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x7E, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x01, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0x80, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x01, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xC0, 0x00, 0xFE, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0x80, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x00, 0xFF, 0xFF, 0x00, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xC0, 0x01, 0xFE, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0x80, 0x07, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x00, 0x3F, 0xFC, 0x00, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x01, 0xFE, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x0F, 0xE0, 0x00, 0x00
, 0x00, 0x0F, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFE, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



// RowLength = 128
PROGMEM const uint8_t bitmapSae[912] = 
{
	0x00, 0x00, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x07, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x80
	, 0x00, 0x1F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x80
	, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x80
	, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
	, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
	, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
	, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
	, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0xFF, 0xFE, 0x00
	, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0x00
	, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0x00
	, 0x03, 0xFF, 0xFE, 0x07, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0x00
	, 0x07, 0xFF, 0xFC, 0x03, 0xFF, 0xF8, 0x0F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xE0, 0x00, 0x04, 0x00
	, 0x07, 0xFF, 0xF8, 0x01, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xE0, 0x00, 0x00, 0x00
	, 0x07, 0xFF, 0xF8, 0x01, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00
	, 0x07, 0xFF, 0xFC, 0x00, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00
	, 0x07, 0xFF, 0xFE, 0x00, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00
	, 0x07, 0xFF, 0xFF, 0x80, 0xFF, 0xF8, 0x7F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00
	, 0x03, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xF0, 0x1F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00
	, 0x03, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFD, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00
	, 0x01, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0xFF, 0xFC, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00
	, 0x01, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xF8, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xF8, 0x00
	, 0x00, 0xFF, 0xFF, 0xFF, 0xE0, 0x01, 0xFF, 0xF8, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xF0, 0x00
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x03, 0xFF, 0xFF, 0xFE, 0x07, 0xFF, 0xE0, 0x7F, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00
	, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xE0, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00
	, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0x0F, 0xFF, 0xC0, 0x7F, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00
	, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x9F, 0xFF, 0xC0, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFF, 0xE0, 0x00
	, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFE, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFC, 0x00, 0x00, 0x00, 0x00
	, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00
	, 0xFF, 0xFE, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00
	, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00
	, 0xFF, 0xFF, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00
	, 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0x80, 0x00
	, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00
	, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00
	, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00
	, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00
	, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
	, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
	, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
	, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
	, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xF8
	, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 0x04
	, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x04, 0xF2
	, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x04, 0x83
	, 0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x0C, 0x91
	, 0x00, 0x3F, 0xFF, 0xF8, 0x7F, 0xFE, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x08, 0xF1
	, 0x00, 0x03, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x81
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x92
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C
	, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0
};
