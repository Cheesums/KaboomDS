#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>
#include <nf_lib.h>

volatile int paddleState = 0;
volatile int frame = 0;

typedef struct{u8 ID, X, Y, Frame;} Sprite_Data;

Sprite_Data Sprite;

int main() {

	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

	NF_SetRootFolder("NITROFS");

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);

	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);

	NF_LoadTiledBg("bg/backgroundTop", "backgroundTop", 256, 256);
	NF_LoadTiledBg("bg/backgroundBot", "backgroundBot", 256, 256);
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");

	NF_LoadSpriteGfx("sprite/runnerSpriteSheet", 0, 8, 8);
	NF_LoadSpritePal("sprite/runnerSpriteSheet", 0);

	NF_VramSpriteGfx(0, 0, 0, false);
	NF_VramSpritePal(0, 0, 0);

	u8 X, Y, ID = 0;

	NF_CreateSprite(0,0,0,0,0,0);

	touchPosition Stylus;

	while(1) {

		
		scanKeys();
		touchRead(&Stylus);

		if (Stylus.px > Sprite.X)
		{
			Sprite.X++;
			if ((frame%20) < 10)
			{
				NF_SpriteFrame(0, 0, 1);
			} else
			{
				NF_SpriteFrame(0, 0, 0);
			}
			
		} else if (Stylus.px < Sprite.X)
		{
			Sprite.X--;
			if ((frame%20) < 10)
			{
				NF_SpriteFrame(0, 0, 2);
			} else
			{
				NF_SpriteFrame(0, 0, 3);
			}
		}else
		{
			NF_SpriteFrame(0, 0, 0);
		}
		

		if (Stylus.py > Sprite.Y)
		{
			Sprite.Y++;
		} else if (Stylus.py < Sprite.Y)
		{
			Sprite.Y--;
		}
		
		NF_MoveSprite(0, 0, Sprite.X, Sprite.Y);
		
		frame++;

		NF_SpriteOamSet(0);
		swiWaitForVBlank();
		oamUpdate(&oamMain);

	}

	return 0;
}
