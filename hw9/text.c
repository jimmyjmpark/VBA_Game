#include "myLib.h"
#include "text.h"

void drawChar(int row, int col, char ch, int size, u16 color)
{
	for(int r=0; r<8; r++) {
		for(int c = 0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6)+ch*48]) {
				for(int hs = 0; hs < size; hs++) {
					for(int ws = 0; ws < size; ws++) {
						setPixel(row+r*size+hs, col+c*size+ws, color);
					}
				}
			}
		}
	}
}

void drawString(int row, int col, char *str, int size, u16 color)
{
	while(*str)
	{
		drawChar(row, col, *str++, size, color);
		col += 6*size;
	}
}

void drawStringMiddle(int row, char *str, int charLength, int size, u16 color)
{
	int col = 120-charLength*6*size/2;
	drawString(row, col, str, size, color);
}
