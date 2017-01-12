#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;


void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{

	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int width, int height, const unsigned short *image)
{
		REG_DMA3CNT=0;
	for(int r = 0; r < height; r++)
	{
		DMA[DMA_CHANNEL_3].src = r * width + image;
		DMA[DMA_CHANNEL_3].dst = &videoBuffer[r * width];
		DMA[DMA_CHANNEL_3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT;
	}
}

void drawImage(int x, int y, int width, int height, const unsigned short *image) {
	// TODO Draw the image with the top left corner at (x,y).
	// Recall that the GBA screen is 240 pixels wide, and 160 tall.
	// The videoBuffer is a 1-dimensional array of these pixels.
	// For example, to set pixel (37,21) to white:
	//   videoBuffer[21 * 240 + 37] = 0x7FFF;
	for(int k = 0; k < height; k++) {
		for(int j = 0; j < width; j++) {
			videoBuffer[OFFSET(k+y,j+x,240)] = *(unsigned short*)(image + j + k * width);
		}
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

int checkCollision(PLAYER player, MOVOBJ objs) {
	int hit = 0;

	//left bottom of player
	for(int j = 0; j < player.hsize; j++) {
			if(player.col <= objs.col+objs.wsize && player.col >= objs.col
					&& player.row+player.hsize >= objs.row && player.row+player.hsize <= objs.row+objs.hsize) {
						hit = 1;
					}
	}

	//right bottom of player
	for(int j = 0; j < player.hsize; j++) {
			if(player.col+player.wsize <= objs.col+objs.wsize && player.col+player.wsize >= objs.col
					&& player.row+player.hsize >= objs.row && player.row+player.hsize <= objs.row+objs.hsize) {
						hit = 1;
					}
	}

	return hit;
}
