#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>

volatile int paddleState = 0;
volatile int frame = 0;
volatile int bomberMult = 1;
int highBucket = 96;

volatile int paddle = 0;
volatile int lastPaddle = 0;
volatile int paddleDis = 0;
volatile int screenDis = 0;

typedef struct{int ID, X, Y, Frame;} Sprite_Data;

Sprite_Data bomber;
Sprite_Data bucket[3];

int main() {

	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

	NF_SetRootFolder("NITROFS");

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);

	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);
	NF_InitSpriteSys(1);

	NF_LoadTiledBg("bg/backgroundTop", "backgroundTop", 256, 256);
	NF_LoadTiledBg("bg/backgroundBot", "backgroundBot", 256, 256);
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");

	bomber.ID = 0;
	NF_LoadSpriteGfx("sprite/bomberSprite", bomber.ID, 64, 64);
	NF_LoadSpritePal("sprite/bomberSprite", bomber.ID);
	NF_VramSpriteGfx(0, 0, 0, false);
	NF_VramSpritePal(0, 0, 0);
	bomber.X = 100;
	bomber.Y = 52;
	NF_CreateSprite(0,bomber.ID, 0, 0, bomber.X ,bomber.Y);

	NF_SpriteFrame(0, bomber.ID, 1);

	NF_LoadSpriteGfx("sprite/bucket", 1, 64, 32);
	NF_LoadSpritePal("sprite/bucket", 1);
	NF_VramSpriteGfx(1, 1, 1, false);
	NF_VramSpritePal(1, 1, 1);

	lastPaddle = paddleRead();

	for (int i = 0; i < 3; i++)
	{
		bucket[i].X = 10;
		bucket[i].Y = (highBucket + (32*i));
		bucket[i].ID = i+1;
		NF_CreateSprite(1, bucket[i].ID, 1, 1, bucket[i].X, bucket[i].Y);
	}


	touchPosition Stylus;

	while(1) {
		
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
		
		screenDis = paddleDis;

		bucket[0].X = bucket[0].X + screenDis;

		if (bucket[0].X < 0)
		{
			bucket[0].X = 0;
		} else if (bucket[0].X > 192)
		{
			bucket[0].X = 192;
		}

		for (int i = 0; i < 3; i++)
		{
			NF_MoveSprite(1, bucket[i].ID, bucket[0].X, bucket[i].Y);
		}
		

		bomber.X = bomber.X + 2*bomberMult;
		int r = rand()%200;
		if (bomber.X > 190 || bomber.X < 2 || r <2)
		{
			bomberMult=bomberMult * -1;
		}
		
		NF_MoveSprite(0, bomber.ID, bomber.X, bomber.Y);

		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

	}

	return 0;
}
