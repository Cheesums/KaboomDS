#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>
#include "sprites.h"
#include "setup.h"



volatile int paddleState = 0;
volatile int frameCount = 0;
int bucketTop = 96;

volatile int paddle = 0;
volatile int lastPaddle = 0;
volatile int paddleDis = 0;
volatile int screenDis = 0;

volatile int bombCount = 0;

typedef struct{int Screen, ID, X, Y, Frame;} Sprite_Data;


int main() {

	MadBomber bomber;
	Bucket bucket[3];
	Bomb bomb[50];

	setup(bomber, bucket, bucketTop); //load and create background and sprites and set initial values

//set paddle to have initial displacement of 0
	lastPaddle = paddleRead();

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

		//move all buckets according to the paddle
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

		//drop a bomb every 20 frames and keep track of each bomb
		if (frameCount%20 == 0)
		{
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
			if (bomb[i].getID() < 55) //only manipulate after a bomb object has been filled (i.e. ID brought into the normal range)
			{
				bomb[i].bombScroll();
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