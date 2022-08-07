#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>
#include "sprites.h"
#include "setup.h"
#include "input.h"
#include "game.h"

int SCREEN_SIZE_Y = 192;
int SCREEN_SIZE_X = 256;

volatile int paddleState = 0;

volatile int frameCount = 0;
int storedFrameCount = 0;

volatile int scoreInt = 0;

int BOMBER_WIDTH = 14;
int BOMBER_SPRITE_X_OFFSET = 1;
int BOMBER_SPAWN_Y = 68;

int BUCKET_TOP = 138;
int BUCKET_HEIGHT = 7;
int BUCKET_WIDTH  = 28;
int BUCKET_OFFSET = 18;
int BUCKET_SPRITE_Y_OFFSET = 9;
int BUCKET_SPRITE_X_OFFSET = 0;

int BOMB_WIDTH = 10;
int BOMB_HEIGHT = 8;
int BOMB_SPRITE_Y_OFFSET = 8;
int BOMB_SPRITE_X_OFFSET = 3;

volatile int paddleDis = 0;

volatile int bombCount = 0;
int bombsCaught = 0;
int storedBombID = 0;
int bombExpFrameCount = 0;

bool bombsExist = false;
bool fusePlaying = false;

int explosionSound;

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

int fuseRando;
int fuseSound;

int START_ROUND_BUTTON = (KEY_START | KEY_A | KEY_L);
int RESET_BUTTON = KEY_START;

extern MadBomber bomber;
extern Bucket bucket[3];
extern Bomb heldBomb;


Paddle paddle;

int main() {

	Bomb bomb[50];


	setup(); //load and create background and sprites and set initial values

	//Setup noise to use for fuses
	fuseSound = soundPlayNoise(22050, 0, 64);
	//Move somewhere nicer once you know what you're doing

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
				frameCount = 0;
				bombsExist = false;
				gameState = 1;
				heldBomb.hide();
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
				bomber.bounce(roundVar[currentRound].revFreq);
			}			
			bomber.updatePos();
		    if ((scoreInt >= 1000) && !(bomber.getFrame()==2)) //give bomber suprise face if score > 1000
                {
                    bomber.setFrame(2);
                }

			//drop a bomb every 20 frames and keep track of each bomb
			if ((frameCount%roundVar[currentRound].bombFrequency == 1) && (roundBombCurrent < roundVar[currentRound].bombTarget))
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

			//check that at least one bomb is on screen
			bombsExist = false;
			
			for (int i = 0; i < 50; i++)
			{
				if (bomb[i].isSpawned()) //only manipulate after a bomb object has been filled (i.e. isSpawned is true)
				{
					bomb[i].bombScroll(roundVar[currentRound].bombVelY);
					bombsExist = true;

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
							bombsExist = false;
							storedFrameCount = frameCount;
							storedBombID = i;
							bombRollover = (storedBombID > bombCount);
							bomber.setFrame(1);
							gameState = 3;
						}
					}
				}
			}
			//Play random fuse noise
			//This sucks rn but its kinda close. I think its too fuzzy? no idea how to fix
			if (bombsExist)
			{
				fuseRando = rand()%4;
				if (fusePlaying)
				{
					fusePlaying = false;
				}else
				{			
			
					fusePlaying = true;
					switch (fuseRando)
					{
					case 0:
						//F:3, V:3
						soundSetFreq(fuseSound, 19845);
						soundSetVolume(fuseSound, 3);
						break;
					
					case 1:
						//F:1, V:2
						soundSetFreq(fuseSound, 15435);
						soundSetVolume(fuseSound, 2);
						break;

					case 2:
						//F:2, V:2
						soundSetFreq(fuseSound, 17640);
						soundSetVolume(fuseSound, 2);
						break;

					case 3:
						//F:0, V:1
						soundSetFreq(fuseSound, 13230);
						soundSetVolume(fuseSound, 1);
						break;

					default:
						break;
					}
				}
			} else
			{
				soundSetVolume(fuseSound, 0);
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
				frameCount = 0;
				heldBomb.hide();
				gameState = 1;
			}
			
			

			break;
		case 3:
		//Perform round loss animation and prepare consequences of the loss (delete a bucket or end the game)

			//Perform each case all in a row one case per frame
			switch (bombExpFrameCount)
			{
			case 0:
				//Set the bottom bomb to the first explosions sprite
				NF_PlayRawSound(((explosionSound%5)+8), 127, 64, false, 0);
				bomb[storedBombID].setFrame(4);
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
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 4:
				//wait				//explosions sprite 2
				bomb[storedBombID].setFrame(5);
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 5:
				//wait a frame
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 6:
				bomb[storedBombID].cycleColor();
				bombExpFrameCount++;
				break;
			case 7:
				//wait
				//explosion sprite 3
				bomb[storedBombID].setFrame(6);
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
				explosionSound++;
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
					//To be added - delay to seperate screen explosion further from bomb explosions
					explosionSound = 0;
					NF_PlayRawSound(13, 127, 64, false, 0);
					explosion();
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
					bomber.setFrame(0);
					heldBomb.setX(bomber.getX());
					heldBomb.updatePos();
					heldBomb.setX(bomber.getX());
					heldBomb.updatePos();
					heldBomb.show();
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
				heldBomb.setX(bomber.getX());
				heldBomb.updatePos();
				heldBomb.show();
				gameState = 0;
				remainingBuckets = 3;
				bomber.setFrame(0);
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

		sprintf(scoreString, "score %i", scoreInt);
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