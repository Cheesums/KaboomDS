#include "game.h"
#include "setup.h"

extern int BUCKET_TOP;
extern int BUCKET_HEIGHT;
extern int BUCKET_WIDTH;
extern int BUCKET_OFFSET;

extern int remainingBuckets;
extern int bombsCaught;

extern int BOMB_WIDTH;
extern int BOMB_HEIGHT;
extern int BOMB_SPRITE_Y_OFFSET;
extern int BOMB_SPRITE_X_OFFSET;
extern int BUCKET_SPRITE_Y_OFFSET;
extern int BUCKET_SPRITE_X_OFFSET;

int newLifeTracker;

extern int scoreInt;
extern int gameState;
extern int currentRound;

extern int catchBucket;
extern int catchFrame;

extern int fuseSound;

int catchSoundChannel;

extern Bucket bucket[3];
extern RoundVar roundVar[9];
extern MadBomber bomber;
extern Bomb heldBomb;

void collision(Bomb &bomb) {

    int bucketCounter = 0;
    int bucketTop = BUCKET_TOP + BUCKET_SPRITE_Y_OFFSET;
    int bucketBot = bucketTop + (remainingBuckets*BUCKET_OFFSET);

    int bucketX = bucket[0].getX() + BUCKET_SPRITE_X_OFFSET;
    int bombX = bomb.getX() + BOMB_SPRITE_X_OFFSET;
    int bombY = bomb.getY() + BOMB_SPRITE_Y_OFFSET;

    //check if bomb is withing X range of the buckets
    if ((bombX+BOMB_WIDTH >= bucketX) && (bombX <= bucketX+BUCKET_WIDTH))
    {

        for(bucketCounter=0; bucketCounter<remainingBuckets; bucketCounter=bucketCounter+1)
        {
            bucketTop = BUCKET_TOP+BUCKET_SPRITE_Y_OFFSET + BUCKET_OFFSET*bucketCounter;

            if (((bombY+BOMB_HEIGHT >= bucketTop) && (bombY <= bucketTop+BUCKET_HEIGHT)) && bomb.isSpawned())   //Bomb is caught
            {
                bomb.del();
                soundKill(catchSoundChannel);
                catchSoundChannel = NF_PlayRawSound(currentRound, 127, 64, false, 0);
                bombsCaught++;
                scoreInt = scoreInt + roundVar[currentRound].bombValue;

               catchFrame = 0;
               catchBucket = bucketCounter;
                
                newLifeTracker = newLifeTracker + roundVar[currentRound].bombValue;
                if (newLifeTracker >= 1000)
                {
                    newLifeTracker = newLifeTracker - 1000;
                    if (remainingBuckets < 3)
                    {
                        bucket[remainingBuckets].show();
                        NF_PlayRawSound(14, 127, 64, false, 0);
                        remainingBuckets++;
                    }
                    
                }
                


                if (bombsCaught >= roundVar[currentRound].bombTarget)   //Last bomb for the round is caught
                {
                    soundPause(fuseSound);
					heldBomb.setX(bomber.getX());
					heldBomb.updatePos();
                    heldBomb.show();
                    gameState = 2;
                    currentRound++;
                    if (currentRound > 7)
                    {
                        currentRound = 7;
                    }
                    
                }
            }
            
        }
        /*
        for (bucketTop = BUCKET_TOP + BUCKET_SPRITE_Y_OFFSET; bucketTop < bucketBot; bucketTop = bucketTop + BUCKET_OFFSET)
        {
            if (((bombY+BOMB_HEIGHT >= bucketTop) && (bombY <= bucketTop+BUCKET_HEIGHT)) && bomb.isSpawned())   //Bomb is caught
            {
                bomb.del();
                soundKill(catchSoundChannel);
                soundSetVolume(fuseSound, 0);
                catchSoundChannel = NF_PlayRawSound(currentRound, 127, 64, false, 0);
                bombsCaught++;
                scoreInt = scoreInt + roundVar[currentRound].bombValue;
                
                newLifeTracker = newLifeTracker + roundVar[currentRound].bombValue;
                if (newLifeTracker >= 1000)
                {
                    newLifeTracker = newLifeTracker - 1000;
                    if (remainingBuckets < 3)
                    {
                        bucket[remainingBuckets].show();
                        NF_PlayRawSound(14, 127, 64, false, 0);
                        remainingBuckets++;
                    }
                    
                }
                


                if (bombsCaught >= roundVar[currentRound].bombTarget)   //Last bomb for the round is caught
                {
                    soundSetVolume(fuseSound, 0);
					heldBomb.setX(bomber.getX());
					heldBomb.updatePos();
                    heldBomb.show();
                    gameState = 2;
                    currentRound++;
                    if (currentRound > 8)
                    {
                        currentRound = 8;
                    }
                    
                }
                
            }
            
            
        }
        */
    }
    
}

//Change background and text color for explosion. Touch up colors later
void explosion() {
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,12,25,12);
    NF_BgSetPalColor(0,3,1,12,25,12);
    NF_BgSetPalColor(0,3,2,22,22,5);
    NF_BgSetPalColor(0,0,1,12,12,12);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,10,22,10);
    NF_BgSetPalColor(0,3,1,10,22,10);
    NF_BgSetPalColor(0,3,2,2,25,0);
    NF_BgSetPalColor(0,0,1,16,16,16);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,16,31,16);
    NF_BgSetPalColor(0,3,1,16,31,16);
    NF_BgSetPalColor(0,3,2,28,28,8);
    NF_BgSetPalColor(0,0,1,0, 0, 0);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,14,28,14);
    NF_BgSetPalColor(0,3,1,14,28,14);
    NF_BgSetPalColor(0,3,2,3,31,9);
    NF_BgSetPalColor(0,0,1,8,8,8);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,2,11,2);
    NF_BgSetPalColor(0,3,1,2,11,2);
    NF_BgSetPalColor(0,3,2,7,7,0);
    NF_BgSetPalColor(0,0,1,26,26,25);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,0,7,0);
    NF_BgSetPalColor(0,3,1,0,7,0);
    NF_BgSetPalColor(0,3,2,11,11,1);
    NF_BgSetPalColor(0,0,1,28,28,28);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,7,18,7);
    NF_BgSetPalColor(0,3,1,7,18,7);
    NF_BgSetPalColor(0,3,2,15,15,3);
    NF_BgSetPalColor(0,0,1,20,20,20);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,5,15,5);
    NF_BgSetPalColor(0,3,1,5,15,5);
    NF_BgSetPalColor(0,3,2,19,19,4);
    NF_BgSetPalColor(0,0,1,23,23,22);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,18,25,11);
    NF_BgSetPalColor(0,3,1,18,25,11);
    NF_BgSetPalColor(0,3,2,20,20,20);
    NF_BgSetPalColor(0,0,1,15,15,3);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,15,21,9);
    NF_BgSetPalColor(0,3,1,15,21,9);
    NF_BgSetPalColor(0,3,2,23,23,22);
    NF_BgSetPalColor(0,0,1,19,19,4);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,24,31,15);
    NF_BgSetPalColor(0,3,1,24,31,15);
    NF_BgSetPalColor(0,3,2,26,26,25);
    NF_BgSetPalColor(0,0,1,7,7,0);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,21,28,13);
    NF_BgSetPalColor(0,3,1,21,28,13);
    NF_BgSetPalColor(0,3,2,28,28,28);
    NF_BgSetPalColor(0,0,1,11,11,1);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,5,10,2);
    NF_BgSetPalColor(0,3,1,5,10,2);
    NF_BgSetPalColor(0,3,2,0, 0, 0);
    NF_BgSetPalColor(0,0,1,28,28,6);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,2,6,0);
    NF_BgSetPalColor(0,3,1,2,6,0);
    NF_BgSetPalColor(0,3,2,8,8,8);
    NF_BgSetPalColor(0,0,1,31,31,9);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,12,18,7);
    NF_BgSetPalColor(0,3,1,12,18,7);
    NF_BgSetPalColor(0,3,2,12,12,12);
    NF_BgSetPalColor(0,0,1,12,22,5);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,9,14,4);
    NF_BgSetPalColor(0,3,1,9,14,4);
    NF_BgSetPalColor(0,3,2,16,16,15);
    NF_BgSetPalColor(0,0,1,25,25,6);
    swiWaitForVBlank();
    swiWaitForVBlank();
    
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");
	NF_CreateTextLayer16(0, 0, 0, "font");
	NF_DefineTextColor(0,0,0,25,25,6);
}