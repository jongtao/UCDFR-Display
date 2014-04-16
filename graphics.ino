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



void graphics_blit(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char *bitmap, unsigned char width,
	unsigned char height)
{
	unsigned char page1, page2, cs, horizontal, i, j, k;

	page1 = y / LCD_PAGES; // find page to start
	k = y - LCD_PAGES * page1; // offset after page


	for(j=0; j<height; j++)
	{
		page2 = (y + j) / LCD_PAGES; // compute new page

		if(page2 >= LCD_PAGES) // crop bottom if too low
			return;

		if(page1 != page2)
		{
			page1 = page2; // increment page
			k=0;
		} // if different page
/* off set bitmap[j] according to width and height
		for(i=0; i<width; i++)
		{
			horizontal = i + x; // current (horizontal) pixel being drawn

			if(horizontal >= LCD_WIDTH)
				break;

			if(horizontal >= 64)
			{
				cs = 0;
				horizontal -= 64;
			} // if need to move to next CS
			else
				cs = 1;

			lcdBuffer[cs][page2][horizontal] |=
				(!!(bitmap[j] & (0x01 << w-1-i))) << k;
		} // for horizontal pixel in char
*/
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
