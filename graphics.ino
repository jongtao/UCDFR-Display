#include "graphics.h"


// CAUTION: beware for uint8_t range
void graphics_blit(uint8_t lcdBuffer[2][8][64],
	uint8_t dstX, uint8_t dstY, uint8_t *bitmap,
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
			//lcdBuffer[cs][page2][horizontal] |=
				//(!!(bitmap[indY + indX] & (0x01 << (7 - (i + srcX)%8)))) << k; // BLIT
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
} // graphics_blit_text()
