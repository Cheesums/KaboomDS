#include "setup.h"

extern MadBomber bomber;
extern Bucket bucket[3];

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
void spawnBuckets(int bucketTop) {
	for (int i = 0; i < 3; i++)
	{
		bucket[i].setX(10);
		bucket[i].setY(bucketTop + (32*i));
		bucket[i].setID(i+1);
		bucket[i].setScreen(1);
		bucket[i].setGfx(1);
		bucket[i].setPal(1);
		bucket[i].create();
		//NF_CreateSprite(1, bucket[i].ID, 1, 1, bucket[i].X, bucket[i].Y);
	}
}

void setup(int bucketTop) {
    loadGraphics();
    displayBackgrounds();    

    //populate and spawn all buckets in the array
    spawnBuckets(bucketTop);

    //create the bober and set initial values
	bomber.setID(0);
	bomber.setPos(100, 52);
	bomber.setScreen(0);
	bomber.setGfx(0);
	bomber.setPal(0);
	bomber.create();
	bomber.setLayer(3);
    //change the bomber sprite to the frame holding the bomb
	bomber.setFrame(1);
    //set bomber scroll speed
	bomber.setVel(2);
}