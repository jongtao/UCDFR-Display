#include "graphics.h"


void graphics_blit_char(unsigned char lcdBuffer[2][8][64],
	unsigned char x, unsigned char y, unsigned char character[])
{
	unsigned char page, i, j;
	y=(y-1)/8;

	for(j=0; j<9; j++)
	{
		if(j>8)
			page = y + 1;
		else
			page = y;

		for(i=0; i<8; i++)
			lcdBuffer[1][page][i + x] ^= (!!(character[j] & (0x01<< 7-i))) << j;
	}
}
