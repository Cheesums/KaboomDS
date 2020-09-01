#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>
#include "sprites.h"
#include "setup.h"
#include "input.h"
#include "game.h"



volatile int paddleState = 0;
volatile int frameCount = 0;

volatile int scoreInt = 0;

int BUCKET_TOP = 96;
int BUCKET_HEIGHT = 16;
int BUCKET_WIDTH  = 64;
int BUCKET_OFFSET = 32;

int BOMB_WIDTH = 16;
int BOMB_HEIGHT = 16;

volatile int paddleDis = 0;

volatile int bombCount = 0;

char scoreChar = '0';
char* scoreString;

extern MadBomber bomber;
extern Bucket bucket[3];

Paddle paddle;

int main() {

	Bomb bomb[50];

	setup(); //load and create background and sprites and set initial values


//perform everything in loop once each frame
	while(1) {
		
		//get paddle displacement
		paddleDis = paddle.getPaddleDis();


		//move all buckets according to the paddle
		for (int i = 0; i < 3; i++)
		{
			bucket[i].bucketScroll(paddleDis);
		}
		
		//move the bomber around the screen
		//make the bomber bounce off the edged of the screen and randomly change direction without bumbing the wall
		bomber.move(bomber.getVel(), 0);
		bomber.screenBounce();
		bomber.updatePos();

		//drop a bomb every 20 frames and keep track of each bomb
		if (frameCount%20 == 0)
		{
			bomb[bombCount].spawn(bombCount);
			bombCount++;
			if (bombCount == 50)
			{
				bombCount = 0;
			}
		}
		
		//Move bombs downscreen and recreate bombs on the bottom screen when they fall off of the top screen.
		//Only move bombs if they are within the expected bomb ID range
		//Hide bombs and move them offscreen when they fall off of the bottom screen.
		for (int i = 0; i < 50; i++)
		{
			if (bomb[i].getID() < 55) //only manipulate after a bomb object has been filled (i.e. ID brought into the normal range)
			{
				bomb[i].bombScroll();
			}

			if (bomb[i].getScreen() == 1)
			{
				collision(bomb[i]);
			}

			if (bomb[i].getY() > 198)
			{
				if (bomb[i].getScreen() < 1)
				{
					bomb[i].jumpScreen();
				}else
				{
					bomb[i].hide();
				}
			}
		}
		
		sprintf(scoreString, "%d", scoreInt);
		NF_WriteText16(0, 0, 0, 0, scoreString);
		

		frameCount ++;
		//update both screens
		NF_UpdateTextLayers();
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}

	return 0;
}