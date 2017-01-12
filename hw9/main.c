// Name: Jaemin Park
#include "stdlib.h"
#include "stdio.h"
#include "myLib.h"
#include "text.h"
#include "Game_Over.h"
#include "Start_Screen.h"
#include "Sun.h"
// Useful defines

// State enum definition
enum GBAState {
	START_SCREEN,
	START_NODRAW,
	GAME_OVER,
	GG_NODRAW,
	STATE_NEWGAME,
	STATE_PAUSE,
	STATE_GAME
};

int main() {
  int stageFloor = 130;

  u16 bgcolor = CYAN;
  REG_DMA3SAD = (u32)&bgcolor;
  REG_DMA3DAD = (u32)videoBuffer;
  REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawRect(stageFloor,0,160-stageFloor,240,BROWN);

	int rd = 4;
	int jumpStrength = 13;

	//int randCount = 0;
	int sleepCounter;
  int counter = 0;
	int powerCounter = 0;
	int obd = 1;
	int score = (counter-powerCounter)/20;

	int obhsize = 30;
	int obwsize = 10;
  int obRposition = stageFloor-obhsize;
  int obCposition = 240-obwsize;

	int NUMOBJS = 5;
	int NUMOBJSAPP = 0;
	int NUMOBJSDIS = 0;
	int oldNUMOBJSAPP = NUMOBJSAPP;
	int oldNUMOBJSDIS = NUMOBJSDIS;
	int super = 0;

	int stage = 1;
	PLAYER player;
	PLAYER oldPlayer;

	player.row = 10;
	player.col = 10;
	player.hsize = 20;
	player.wsize = 10;
	player.color = RED;

	oldPlayer.row = player.row;
	oldPlayer.col = player.col;

	MOVOBJ objs[50];
	MOVOBJ oldobjs[50];
	MOVOBJ *cur;

	int startButton = 0;
	int oldstartButton = startButton;
	int selectButton = 0;
	int oldselectButton = selectButton;
	char buffer[41];
	enum GBAState state = START_SCREEN;

	while(1) {
		switch(state) {
			case START_SCREEN:
//			  REG_DMA3SAD = (u32)&bgcolor;
//			  REG_DMA3DAD = (u32)videoBuffer;
//			  REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;
				drawImage3(START_SCREEN_WIDTH, START_SCREEN_HEIGHT, Start_Screen_data);
				REG_DISPCTL = MODE3 | BG2_ENABLE;
//				drawRect(stageFloor,0,160-stageFloor,240,BROWN);
				state = START_NODRAW;


				drawStringMiddle(START_R_POSITION, "ADVENTURE OF", sizeof("ADVENTURE OF"), START_SIZE, BLACK);
				drawStringMiddle(START_R_POSITION+25, "RED SQUARE", sizeof("RED SQUARE"), START_SIZE, BLACK);
				drawStringMiddle(START_R_POSITION+50, "PRESS START", sizeof("PRESS START"), START_SIZE-1, BLACK);
				drawStringMiddle(START_R_POSITION+70, "TO CONTINUE", sizeof("TO CONTINUE"), START_SIZE-1, BLACK);
				break;

			case START_NODRAW:
				if(KEY_DOWN_NOW(BUTTON_START)) {
					startButton = 1;
				}
				else {
					startButton = 0;
				}

				if(startButton == 1 && oldstartButton == 0) {
					state = STATE_NEWGAME;
					counter = 0;
					stage = 1;
					powerCounter = 0;
				}
			break;

			case GAME_OVER:
				REG_DMA3SAD = (u32)&bgcolor;
				REG_DMA3DAD = (u32)videoBuffer;
				REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;
				REG_DISPCTL = MODE3 | BG2_ENABLE;
				drawRect(stageFloor,0,160-stageFloor,240,BROWN);
				state = GG_NODRAW;
				drawImage3(GAME_OVER_WIDTH, GAME_OVER_HEIGHT, Game_Over_data);
				drawString(START_R_POSITION,110, "GAME OVER", START_SIZE-1, BLACK);
				sprintf(buffer, "Final Score : %d", score);
				drawString(START_R_POSITION+15,110, buffer, START_SIZE-2, BLACK);
				drawString(START_R_POSITION+100, 10, "PRESS START", START_SIZE-1, BLACK);
				drawString(START_R_POSITION+115, 10, "TO PLAY AGAIN", START_SIZE-1, BLACK);
				break;

			case GG_NODRAW:
				if(KEY_DOWN_NOW(BUTTON_START)) {
					startButton = 1;
				}
				else {
					startButton = 0;
				}

				if(startButton == 1 && oldstartButton == 0) {
					state = STATE_NEWGAME;
					counter = 0;
					stage = 1;
					powerCounter = 0;
				}
			break;

			case STATE_NEWGAME:
				state = STATE_GAME;
				REG_DMA3SAD = (u32)&bgcolor;
				REG_DMA3DAD = (u32)videoBuffer;
				REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;
				REG_DISPCTL = MODE3 | BG2_ENABLE;
				drawRect(stageFloor,0,160-stageFloor,240,BROWN);

				drawImage(200, 0,SUN_WIDTH, SUN_HEIGHT, Sun_data);
				rd = 5;

				obhsize = 30;
				obwsize = 10;
				obRposition = stageFloor-obhsize;
				obCposition = 240-obwsize;

				score = (counter-powerCounter)/20;

				NUMOBJS = 3 + 2 * stage;

				if(NUMOBJS >= 50) {
					NUMOBJS = 50;
				}

				NUMOBJSAPP = 0;
				NUMOBJSDIS = 0;
				oldNUMOBJSAPP = NUMOBJSAPP;
				oldNUMOBJSDIS = NUMOBJSDIS;

				player.row = 10;
				player.col = 10;
				player.hsize = 20;
				player.wsize = 10;
				player.color = RED;


				for(int i=0; i<NUMOBJS; i++)
				{
					objs[i].row = obRposition;
					objs[i].col = obCposition;
					objs[i].color = BLACK;
					objs[i].appear = 0;
					objs[i].hsize = obhsize;
					objs[i].wsize = obwsize;
					oldobjs[i] = objs[i];
				}

				sleepCounter = 0;
				sprintf(buffer, "Stage %d", stage);
				drawStringMiddle(START_R_POSITION, buffer, 7, 2, BLACK);
				while(sleepCounter < 500) {
					waitForVblank();
					sleepCounter += 1;
				}
				drawStringMiddle(START_R_POSITION, buffer, 7, 2, bgcolor);
				drawRect(150,170,10,70, BROWN);
				sprintf(buffer, "Stage : %d", stage);
				drawString(150, 170, buffer, 1, BLACK);
			break;

			case STATE_PAUSE:
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				selectButton = 1;
			}
			else {
				selectButton = 0;
			}

			if(selectButton == 1 && oldselectButton == 0) {
					state = STATE_GAME;
					drawStringMiddle(PAUSE_R_POSITION+50, "PAUSED", sizeof("PAUSED"), PAUSE_SIZE, bgcolor);
				}
			break;

			case STATE_GAME:
				//player controls
		    if(KEY_DOWN_NOW(BUTTON_UP) && player.row >= stageFloor-player.hsize) {
		        rd = -jumpStrength;
				}
		    if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
		        player.col += 3;
		    }
		    if(KEY_DOWN_NOW(BUTTON_LEFT)) {
		        player.col -= 3;
		    }
				if(KEY_DOWN_NOW(BUTTON_A)) {
					obd = 1;
					powerCounter += 3;
					super = 0;
				}
				else {
					//obd = 1+(stage-1)/2;
					obd = 1+stage;
					super = 1;
				}
				//end player controls

				//player physics
				rd += 1;
				player.row += rd;

				if(player.row > stageFloor-player.hsize) {
					player.row = stageFloor-player.hsize;
				}
				if(player.col < 5) {
					player.col = 5;
				}
				if(player.col > 235-player.wsize) {
					player.col = 235-player.wsize;
				}
				//end player physics

				counter += 1;

				if(counter % (80/stage) == 0 && NUMOBJSAPP < NUMOBJS) {
					NUMOBJSAPP += 1;
				}
				for(int i = NUMOBJSDIS; i < NUMOBJSAPP && i < NUMOBJS ; i++) {
					cur = objs + i;

					cur->col -= obd;
					if(cur->col+cur->wsize <= 0) {
						//cur->appear = 0;
						NUMOBJSDIS += 1;
					}
				}


				waitForVblank();

				//erase old stuff
				//for(int i=; i<NUMOBJS; i++)
				for(int i = oldNUMOBJSDIS; i < oldNUMOBJSAPP && i < NUMOBJS; i++) {
					drawRect(oldobjs[i].row, oldobjs[i].col+oldobjs[i].wsize-obd, oldobjs[i].hsize, obd, bgcolor);
				}
				drawRect(oldPlayer.row, oldPlayer.col, oldPlayer.hsize, oldPlayer.wsize, bgcolor);
				drawRect(150, 0, 10, 70, BROWN);
				//draw new stuff
				for(int i = NUMOBJSDIS; i < NUMOBJSAPP && i < NUMOBJS; i++)	{
					cur = objs + i;
					if(checkCollision(player, *cur)) {
						state = GAME_OVER;
					}

					if(cur->col>0) {
						drawRect(cur->row, cur->col, cur->hsize, obd, cur->color);
					}
					oldobjs[i] = objs[i];
				}
				if(super) {
					drawRect(player.row,player.col,player.hsize,player.wsize,player.color);
				}
				else {
					drawRect(player.row,player.col,player.hsize,player.wsize,BLUE);
				}


				score = (counter-powerCounter)/20;
				if(score < 0) {
					score = 0;
				}
				sprintf(buffer, "Score : %d", score);
				drawString(150, 5, buffer, 1, BLACK);

				oldPlayer = player;
				oldNUMOBJSAPP = NUMOBJSAPP;
				oldNUMOBJSDIS = NUMOBJSDIS;

				if(KEY_DOWN_NOW(BUTTON_START)) {
					startButton = 1;
				}
				else {
					startButton = 0;
				}
				if(startButton == 1 && oldstartButton == 0) {
					state = START_SCREEN;
				}
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					selectButton = 1;
				}
				else {
					selectButton = 0;
				}
				if(selectButton == 1 && oldselectButton == 0) {
					state = STATE_PAUSE;
					drawStringMiddle(PAUSE_R_POSITION+50, "PAUSED", sizeof("PAUSED"), PAUSE_SIZE, BLACK);
				}
			}

			//NEXT STAGE
			if(NUMOBJSDIS >= NUMOBJS) {
				drawRect(player.row, player.col, player.hsize, player.wsize, bgcolor);
				state = STATE_NEWGAME;
				stage += 1;
			}
			oldstartButton = startButton;
			oldselectButton = selectButton;
	}
	return 0;
}
