#include "setup.h"
#include <nf_lib.h>


RoundVar roundVar[9];
extern MadBomber bomber;
extern Bucket bucket[3];
Bomb heldBomb;

extern int BUCKET_TOP;
extern int BUCKET_OFFSET;

extern int BOMBER_SPAWN_Y;

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
//Initialize top screen for text
	NF_InitTextSys(0);

//Load a background for each screen
	NF_LoadTiledBg("bg/background_top", "backgroundTop", 256, 256);
	NF_LoadTiledBg("bg/background_bottom", "backgroundBot", 256, 256);

//Load bomber sprite
	NF_LoadSpriteGfx("sprite/bomber", 0, 16, 32);
	NF_LoadSpritePal("sprite/bomber", 0);
	NF_VramSpriteGfx(0, 0, 0, false);
	NF_VramSpritePal(0, 0, 0);

//Load bucket sprite
	NF_LoadSpriteGfx("sprite/bucket", 1, 32, 16);
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

//Load the font to be used on the text layer of the top screen
NF_LoadTextFont16("fnt/font16", "font", 256, 256, 0);

}

void displayBackgrounds() {
	//Create backgrounds
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");
	NF_CreateTextLayer16(0, 0, 0, "font");
	NF_DefineTextColor(0,0,0,25,25,6);

}

//Spawn buckets with top bucket at initial cooordinates (X, Y)
void spawnBuckets() {

	for (int i = 0; i < 3; i++)
	{
		bucket[i].setX(10);
		bucket[i].setY(BUCKET_TOP + (BUCKET_OFFSET*i));
		bucket[i].setID(i+1);
		bucket[i].setScreen(1);
		bucket[i].setGfx(1);
		bucket[i].setPal(1);
		bucket[i].create();
		//NF_CreateSprite(1, bucket[i].ID, 1, 1, bucket[i].X, bucket[i].Y);
	}
}

void setup() {
    loadGraphics();
    displayBackgrounds();    
	//initialize and load sound
	soundEnable();
	NF_InitRawSoundBuffers();
	NF_LoadRawSound("snd/catch0", 0, 22050, 0);
	NF_LoadRawSound("snd/catch1", 1, 22050, 0);
	NF_LoadRawSound("snd/catch2", 2, 22050, 0);
	NF_LoadRawSound("snd/catch3", 3, 22050, 0);
	NF_LoadRawSound("snd/catch4", 4, 22050, 0);
	NF_LoadRawSound("snd/catch5", 5, 22050, 0);
	NF_LoadRawSound("snd/catch6", 6, 22050, 0);
	NF_LoadRawSound("snd/catch7", 7, 22050, 0);
	NF_LoadRawSound("snd/bomb_explode0", 8, 22050, 0);
	NF_LoadRawSound("snd/bomb_explode1", 9, 22050, 0);
	NF_LoadRawSound("snd/bomb_explode2", 10, 22050, 0);
	NF_LoadRawSound("snd/bomb_explode3", 11, 22050, 0);
	NF_LoadRawSound("snd/bomb_explode4", 12, 22050, 0);
	NF_LoadRawSound("snd/screen_explode", 13, 22050, 0);
	NF_LoadRawSound("snd/new_bucket", 14, 22050, 0);
	//NF_LoadRawSound("snd/bomb_fuse", 15, 22050, 0);
	NF_LoadRawSound("snd/testFiles/fuse0", 15, 33075, 0);
	NF_LoadRawSound("snd/testFiles/fuse1", 16, 33075, 0);
	NF_LoadRawSound("snd/testFiles/fuse2", 17, 33075, 0);
	NF_LoadRawSound("snd/testFiles/fuse3", 18, 33075, 0);
    //populate and spawn all buckets in the array
    spawnBuckets();

	for (int i = 1; i < 9; i++)
	{
		roundVar[i].bomberVelX = 2*i + 1;
		roundVar[i].revFreq = 10 + i;
		roundVar[i].bombValue = i;
		roundVar[i].bombFrequency = 21 - 2*i;
		roundVar[i].bombVelY = i + 1;
		if (i<6)
		{		
			roundVar[i].bombTarget = i*10;
		} else if (i<8)
		{
			roundVar[i].bombTarget = 75 + 25*(i-6);
		} else
		{
			roundVar[i].bombTarget = 150;
		}
		
		
	}
	

    //create the bomber and set initial values
	bomber.setID(0);
	bomber.setPos(100, BOMBER_SPAWN_Y);
	bomber.setScreen(0);
	bomber.setGfx(0);
	bomber.setPal(0);
	bomber.create();
	bomber.setLayer(3);
    //change the bomber sprite frown
	bomber.setFrame(0);
    //set bomber scroll speed
	bomber.setVel(2);

	//create bomb to be held by the bomber
	heldBomb.setID(1);
	heldBomb.setPos(100, BOMBER_SPAWN_Y+18);
	heldBomb.setScreen(0);
	heldBomb.setGfx(2);
	heldBomb.setPal(2);
	heldBomb.create();
	heldBomb.setLayer(2);

}