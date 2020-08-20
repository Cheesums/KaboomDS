#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>
#include "sprites.h"



volatile int paddleState = 0;
volatile int frameCount = 0;
int highBucket = 96;

volatile int paddle = 0;
volatile int lastPaddle = 0;
volatile int paddleDis = 0;
volatile int screenDis = 0;

volatile int bombCount = 0;

typedef struct{int Screen, ID, X, Y, Frame;} Sprite_Data;

void loadGraphics();
void displayBackgrounds();
void spawnBuckets(Bucket bucket[]);

//Sprite_Data bombs[50];

int main() {

	loadGraphics();
	displayBackgrounds();

	MadBomber bomber;
	Bucket bucket[3];
	Bomb bomb[50];


//load and display the bomber
	bomber.setID(0);
	bomber.setPos(100, 52);
	bomber.setScreen(0);
	bomber.setGfx(0);
	bomber.setPal(0);
	bomber.create();
	bomber.setLayer(3);
//change the bomber sprite to the frame holding the bomb
	NF_SpriteFrame(0, bomber.getID(), 1);
	bomber.setVel(2);


//load bucket sprite structs and display 3 buckets
	spawnBuckets(bucket);




//set paddle to have initial displacement of 0
	lastPaddle = paddleRead();

//pre-populate bomb ID's with unused sprite ID so loops don't interfere with non-intended sprites (i.e. moving sprite 0 when sprite 0 is the bomber)
/*
for (int i = 0; i < 50; i++)
{
	//bombs[i].setID(100);
}
*/


//perform everything in loop once each frame
	while(1) {
		//calculate paddle displacement
		//check and correct for wraparound
		paddle = paddleRead();
		paddleDis = paddle - lastPaddle;
		lastPaddle = paddle;
		if (paddleDis > 3000)
		{
			paddleDis = paddleDis - 4095;
		} else if (paddleDis <-3000)
		{
			paddleDis = paddleDis + 4095;
		}
		//scale paddle displacement if buckets move too fast
		screenDis = paddleDis;
		//apply displacement to the top bucket
		//bucket[0].X = bucket[0].X + screenDis;

		//bind buckets within the screen
		/*
		if (bucket[0].X < 0)
		{
			bucket[0].X = 0;
		} else if (bucket[0].X > 192)
		{
			bucket[0].X = 192;
		}
		*/
		//set all buckets to top bucket's position
		for (int i = 0; i < 3; i++)
		{
			//NF_MoveSprite(1, bucket[i].ID, bucket[0].X, bucket[i].Y);
			bucket[i].bucketScroll(screenDis);
		}
		
		//move the bomber around the screen
		//make the bomber bounce off the edged of the screen and randomly change direction without bumbing the wall
		bomber.move(bomber.getVel(), 0);
		bomber.screenBounce();
		bomber.updatePos();

		//drop a bomb every 20 frames
		if (frameCount%20 == 0)
		{
			/*
			bombs[bombCount].Screen = 0;
			bombs[bombCount].ID = bombCount + 4;
			bombs[bombCount].X = bomber.getX();
			bombs[bombCount].Y = 50;
			NF_CreateSprite(0, bombs[bombCount].ID, 2, 2, bombs[bombCount].X, bombs[bombCount].Y);
			*/
			bomb[bombCount].spawn(bombCount, bomber.getX());
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
			if (bomb[i].getID() < 55)
			{
				//bombs[i].Y = bombs[i].Y + 2;
				bomb[i].bombScroll();
			}
			
			if (bomb[i].getY() > 198)
			{
				if (bomb[i].getScreen() < 1)
				{
					/*
					bombs[i].Screen = 1;
					bombs[i].Y = -16;
					NF_CreateSprite(bombs[i].Screen, bombs[i].ID, 2, 2, bombs[i].X, bombs[i].Y);
					*/
					bomb[i].jumpScreen();
				}else
				{
					//NF_ShowSprite(1, bombs[i].ID, false);
					//bombs[i].X = 350;
					bomb[i].hide();
				}
			}
			//bomb[i].updatePos();
			//NF_MoveSprite(bombs[i].Screen, bombs[i].ID, bombs[i].X, bombs[i].Y);
		}
		
		

		frameCount ++;
		//update sprites on both screens
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}

	return 0;
}


void loadGraphics() {

//Prepare both screens for 2D graphics mode
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);
//Use NitroFS filesystem
	NF_SetRootFolder("NITROFS");

//Prepare both screens for backgroudns
	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);
//Prepare both screens for sprites
	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);
	NF_InitSpriteSys(1);

//Load a background for each screen
	NF_LoadTiledBg("bg/backgroundTop", "backgroundTop", 256, 256);
	NF_LoadTiledBg("bg/backgroundBot", "backgroundBot", 256, 256);

//Load bomber sprite
	NF_LoadSpriteGfx("sprite/bomberSprite", 0, 64, 64);
	NF_LoadSpritePal("sprite/bomberSprite", 0);
	NF_VramSpriteGfx(0, 0, 0, false);
	NF_VramSpritePal(0, 0, 0);

//Load bucket sprite
	NF_LoadSpriteGfx("sprite/bucket", 1, 64, 32);
	NF_LoadSpritePal("sprite/bucket", 1);
	NF_VramSpriteGfx(1, 1, 1, false);
	NF_VramSpritePal(1, 1, 1);

//load bomb sprite for both screens
	NF_LoadSpriteGfx("sprite/bomb", 2, 16, 16);
	NF_LoadSpritePal("Sprite/bomb", 2);
	NF_VramSpriteGfx(0, 2, 2, false);
	NF_VramSpritePal(0, 2, 2);
	NF_VramSpriteGfx(1, 2, 2, false);
	NF_VramSpritePal(1, 2, 2);
}

void displayBackgrounds() {
	//Create backgrounds
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");

}

//Spawn buckets with top bucket at initial cooordinates (X, Y)
void spawnBuckets(Bucket bucket[]) {
	for (int i = 0; i < 3; i++)
	{
		bucket[i].setX(10);
		bucket[i].setY(highBucket + (32*i));
		bucket[i].setID(i+1);
		bucket[i].setScreen(1);
		bucket[i].setGfx(1);
		bucket[i].setPal(1);
		bucket[i].create();
		//NF_CreateSprite(1, bucket[i].ID, 1, 1, bucket[i].X, bucket[i].Y);
	}
}