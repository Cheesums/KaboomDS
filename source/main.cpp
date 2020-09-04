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
int storedFrameCount = 0;

volatile int scoreInt = 0;

int BUCKET_TOP = 96;
int BUCKET_HEIGHT = 16;
int BUCKET_WIDTH  = 64;
int BUCKET_OFFSET = 32;

int BOMB_WIDTH = 16;
int BOMB_HEIGHT = 16;

volatile int paddleDis = 0;

volatile int bombCount = 0;
int bombsCaught = 0;
int storedBombID = 0;
int bombExpFrameCount = 0;

char* scoreString;

int gameState = 0;
int currentRound = 0;

int remainingBuckets = 3;

extern RoundVar roundVar[9];

int roundBombCurrent = 0;
bool bombRollover = false;

int Pressed;
int Held;
int Released;

int START_ROUND_BUTTON = (KEY_START | KEY_A | KEY_L);
int RESET_BUTTON = KEY_START;

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
			if ((START_ROUND_BUTTON) & Pressed)
			{
				paddle.setZero();
				currentRound++;
				if (currentRound > 8)
				{
					currentRound = 8;
				}
				roundBombCurrent = 0;
				bomber.setVel(roundVar[currentRound].bomberVelX);
				bombsCaught = 0;
				gameState = 1;
			}
			
			break;
		case 1:
		//playing in round

			//get paddle displacement
			paddleDis = paddle.getPaddleDis();

			//move all buckets according to the paddle
			for (int i = 0; i < 3; i++)
			{
				bucket[i].bucketScroll(paddleDis);
			}
			
			//move the bomber around the screen
			//make the bomber bounce off the edged of the screen and randomly change direction without bumbing the wall
			if (roundBombCurrent < roundVar[currentRound].bombTarget)
			{
				bomber.move(bomber.getVel(), 0);
				bomber.screenBounce();
			}			
			bomber.updatePos();

			//drop a bomb every 20 frames and keep track of each bomb
			if ((frameCount%roundVar[currentRound].bombFrequency == 0) && (roundBombCurrent < roundVar[currentRound].bombTarget))
			{
				
				bomb[bombCount].spawn(bombCount);
				roundBombCurrent++;
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
						collision(bomb[i]);				//overhaul mechanism for winning a round. Probably count how many bombs are captured and compare against round target
					}

					if (bomb[i].getY() > 198)
					{
						if (bomb[i].getScreen() < 1)
						{
							bomb[i].jumpScreen();
						}else
						{
							storedFrameCount = frameCount;
							storedBombID = i;
							bombRollover = (storedBombID > bombCount);
							gameState = 3;
						}
					}
				}
			}
			break;

		case 2:
		//case for waiting between rounds

			paddleDis = paddle.getPaddleDis();
			//move all buckets according to the paddle
			for (int i = 0; i < 3; i++)
			{
				bucket[i].bucketScroll(paddleDis);
			}

			if (START_ROUND_BUTTON & Pressed)
			{
				roundBombCurrent = 0;
				bombsCaught = 0;
				gameState = 1;
			}
			
			

			break;
		case 3:
		//Perform round loss animation and prepare consequences of the loss (delete a buclet or end the game)

			//Perform each case all in a row one case per frame
			switch (bombExpFrameCount)
			{
			case 0:
				//Set the bottom bomb to the first explosions sprite
				bomb[storedBombID].setFrame(0);
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 1:
				//wait a frame
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 2:
				//wait a frame
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 3:
				//explosions sprite 2
				bomb[storedBombID].setFrame(0);
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 4:
				//wait
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 5:
				//wait a frame
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 6:
				//explosion sprite 3
				bomb[storedBombID].setFrame(0);
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 7:
				//wait
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 8:
				//wait a frame
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 9:
				bombExpFrameCount = 0;
				bomb[storedBombID].del();
				//bomb[storedBombID].clearFinal();
				while (!bomb[storedBombID].isSpawned() && ((storedBombID < bombCount) || bombRollover))
				{
					storedBombID++;
						
					if (storedBombID > 49)
					{
						storedBombID = 0;
						bombRollover = false;
					}
				}
				if ((storedBombID >= bombCount) & !bombRollover)
				{
					bombExpFrameCount = 10;
					remainingBuckets = remainingBuckets - 1;
					bucket[remainingBuckets].hide();
				}
				break;
			case 10:
				bombExpFrameCount = 0;
				if (remainingBuckets < 1)
				{
					gameState = 4;
				} else
				{	
					if (currentRound > 1)
					{
						currentRound = currentRound - 1;
					}
					gameState = 2;
				}
				
				
				break;				
			default:
				break;				
			}

						
							
			break;
		case 4:
			//Game Over
			if (RESET_BUTTON & Pressed)
			{
				currentRound = 0;
				scoreInt = 0;
				gameState = 0;
				remainingBuckets = 3;
				for (int i = 0; i < 3; i++)
				{
					bucket[i].show();
				}
				
			}
			break;
		default:
			gameState = 0;
			break;
		}

		sprintf(scoreString, "score %i, stored %i, count %i, gamestate %i, buckets %i", scoreInt, storedBombID, bombCount, gameState, remainingBuckets);
		NF_WriteText16(0, 0, 0, 0, scoreString);
		

		frameCount ++;
		//update both screens
		NF_UpdateTextLayers();
		NF_ClearTextLayer(0, 0);
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}

	return 0;
}