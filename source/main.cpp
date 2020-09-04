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

char* scoreString;

int gameState = 0;
int currentRound = 0;

extern RoundVar roundVar[9];

int roundBombCurrent = 0;

int Pressed;
int Held;
int Released;
extern MadBomber bomber;
extern Bucket bucket[3];

Paddle paddle;

int main() {

	Bomb bomb[50];


	setup(); //load and create background and sprites and set initial values


//perform everything in loop once each frame
	while(1) {
		scanKeys();
		Pressed = keysDown();
		Held = keysHeld();
		Released = keysUp();


		//Determine the state of the game; pre-game, between rounds, or in a roundVar
		switch (gameState)
		{
		case 0:
			if (KEY_A & Pressed)
			{
				paddle.setZero();
				currentRound++;
				if (currentRound > 8)
				{
					currentRound = 8;
				}
				roundBombCurrent = 0;
				bomber.setVel(roundVar[currentRound].bomberVelX);
				gameState = 1;
			}
			
			break;
		case 1:


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
			if ((frameCount%roundVar[currentRound].bombFrequency == 0) && (roundBombCurrent < roundVar[currentRound].bombTarget))
			{
				
				bomb[bombCount].spawn(bombCount);
				roundBombCurrent++;
				if (roundBombCurrent >= roundVar[currentRound].bombTarget)
				{
					bomb[bombCount].setFinal();
					
				}
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
				if (bomb[i].isSpawned()) //only manipulate after a bomb object has been filled (i.e. isSpawned is true)
				{
					bomb[i].bombScroll(roundVar[currentRound].bombVelY);
				
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
							bomb[i].del();
							bomb[i].clearFinal();
						}
					}
				}
			}
			break;

		default:
			gameState = 0;
			break;
		}

		sprintf(scoreString, "score %i, round %i, target %i,   current %i", scoreInt, currentRound, roundVar[currentRound].bombTarget, roundBombCurrent);
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