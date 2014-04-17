#include "graphics.h"


void graphics_blit_char(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char character[8])
{
	unsigned char page1, page2, cs, horizontal, i, j, k;

	page1 = y / 8; // find page to start
	k = y - 8*page1; // offset after page


	for(j=0; j<8; j++)
	{
		page2 = (y + j) / 8; // compute new page

		if(page2 >= 8) // crop bottom if too low
			return;

		if(page1 != page2)
		{
			page1 = page2; // increment page
			k=0;
		} // if different page

		for(i=0; i<8; i++)
		{
			horizontal = i + x; // current (horizontal) pixel being drawn

			if(horizontal >= 128)
				break;

			if(horizontal >= 64)
			{
				cs = 0;
				horizontal -= 64;
			} // if need to move to next CS
			else
				cs = 1;

			lcdBuffer[cs][page2][horizontal] |=
				(!!(character[j] & (0x01 << 8-1-i))) << k;
		} // for horizontal pixel in char

		k++;

	}// for j
} // graphics_blit_char()



// CAUTION: beware for unsigned char range
void graphics_blit(unsigned char lcdBuffer[2][8][64],
	unsigned char dstX, unsigned char dstY, unsigned char *bitmap,
	unsigned char srcX, unsigned char srcY, unsigned char width,
	unsigned char height, unsigned int rowLen)
{
	unsigned char page1, page2, cs, horizontal, i, j, k;
	int indX, indY;

	rowLen = (rowLen- 1)/8 + 1;					// convert pixel length to elements

	page1 = dstY / LCD_PAGES; 							// find page to start
	k = dstY - LCD_PAGES * page1; 					// offset after page

	for(j=0; j<height; j++)
	{
		// Compute page
		page2 = (dstY + j) / LCD_PAGES; 			// compute new page

		if(page2 >= LCD_PAGES) 								// crop bottom if too low
			return;

		if(page1 != page2)
		{
			page1 = page2; 											// increment page
			k=0;
		} // if different page


		// Compute Y index in terms of bitmap location
		indY = (j + srcY)*rowLen;
		//indY = (3)*rowLen;


// off set bitmap[j] according to width and height
		for(i=0; i<width; i++)
		{
			horizontal = i + dstX;							 // horizontal location of pixel being drawn
			
			// Compute LCD width corrections
			if(horizontal >= LCD_WIDTH)
				break;

			if(horizontal >= LCD_CS_WIDTH)
			{
				cs = 0;
				horizontal -= LCD_CS_WIDTH;
			} // if need to move to next CS
			else
				cs = 1;

			
			// Compute X index in terms of bitmap location
			indX = (i + srcX)/8;

		
			lcdBuffer[cs][page2][horizontal] |=
				//(!!(bitmap[indY + 1] & (0x01 << (7 - (i + srcX)%8)))) << k; // BLIT
				(!!(bitmap[indY + indX] & (0x01 << (7 - (i + srcX)%8)))) << k; // BLIT
		} // for horizontal pixel in char

		k++;

	}// for j
} // graphics_blit_char()




void graphics_clear_buffer(unsigned char lcdBuffer[2][8][64])
{
	unsigned char i, j, k;

	for(k=0; k<2; k++)
		for(i=0; i<8; i++)
			for(j=0; j<64; j++)
				lcdBuffer[k][i][j] = 0x00;

} // graphics_clear_buffer()
