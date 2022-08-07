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
                soundSetVolume(fuseSound, 0);
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
    NF_BgSetPalColor(1,3,1,100, 205, 99);
    NF_BgSetPalColor(0,3,1,100, 205, 99);
    NF_BgSetPalColor(0,3,2,180, 180, 42);
    NF_BgSetPalColor(0,0,1,100, 100, 99);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,81, 179, 80);
    NF_BgSetPalColor(0,3,1,81, 179, 80);
    NF_BgSetPalColor(0,3,2,205, 205, 52);
    NF_BgSetPalColor(0,0,1,132, 132, 131);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,134, 253, 133);
    NF_BgSetPalColor(0,3,1,134, 253, 133);
    NF_BgSetPalColor(0,3,2,230, 230, 62);
    NF_BgSetPalColor(0,0,1,0, 0, 0);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,117, 230, 116);
    NF_BgSetPalColor(0,3,1,117, 230, 116);
    NF_BgSetPalColor(0,3,2,253, 253, 72);
    NF_BgSetPalColor(0,0,1,63, 63, 62);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,19, 91, 18);
    NF_BgSetPalColor(0,3,1,19, 91, 18);
    NF_BgSetPalColor(0,3,2,61, 61, 0);
    NF_BgSetPalColor(0,0,1,210, 210, 209);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,0, 57, 0);
    NF_BgSetPalColor(0,3,1,0, 57, 0);
    NF_BgSetPalColor(0,3,2,94, 94, 10);
    NF_BgSetPalColor(0,0,1,234, 234, 233);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,61, 151, 60);
    NF_BgSetPalColor(0,3,1,61, 151, 60);
    NF_BgSetPalColor(0,3,2,123, 123, 21);
    NF_BgSetPalColor(0,0,1,162, 162, 161);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,40, 121, 39);
    NF_BgSetPalColor(0,3,1,40, 121, 39);
    NF_BgSetPalColor(0,3,2,153, 153, 32);
    NF_BgSetPalColor(0,0,1,186, 186, 185);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,149, 203, 89);
    NF_BgSetPalColor(0,3,1,149, 203, 89);
    NF_BgSetPalColor(0,3,2,162, 162, 161);
    NF_BgSetPalColor(0,0,1,123, 123, 21);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,125, 176, 72);
    NF_BgSetPalColor(0,3,1,125, 176, 72);
    NF_BgSetPalColor(0,3,2,186, 186, 185);
    NF_BgSetPalColor(0,0,1,153, 153, 32);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,194, 253, 120);
    NF_BgSetPalColor(0,3,1,194, 253, 120);
    NF_BgSetPalColor(0,3,2,210, 210, 209);
    NF_BgSetPalColor(0,0,1,61, 61, 0);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,173, 229, 105);
    NF_BgSetPalColor(0,3,1,173, 229, 105);
    NF_BgSetPalColor(0,3,2,234, 234, 233);
    NF_BgSetPalColor(0,0,1,94, 94, 10);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,43, 84, 17);
    NF_BgSetPalColor(0,3,1,43, 84, 17);
    NF_BgSetPalColor(0,3,2,0, 0, 0);
    NF_BgSetPalColor(0,0,1,230, 230, 62);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,14, 50, 0);
    NF_BgSetPalColor(0,3,1,14, 50, 0);
    NF_BgSetPalColor(0,3,2,63, 63, 62);
    NF_BgSetPalColor(0,0,1,253, 253, 72);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,99, 147, 54);
    NF_BgSetPalColor(0,3,1,99, 147, 54);
    NF_BgSetPalColor(0,3,2,100, 100, 99);
    NF_BgSetPalColor(0,0,1,180, 180, 42);
    swiWaitForVBlank();
    swiWaitForVBlank();
    NF_BgSetPalColor(1,3,1,71, 115, 35);
    NF_BgSetPalColor(0,3,1,71, 115, 35);
    NF_BgSetPalColor(0,3,2,132, 132, 131);
    NF_BgSetPalColor(0,0,1,205, 205, 52);
    swiWaitForVBlank();
    swiWaitForVBlank();
    
	NF_CreateTiledBg(0, 3, "backgroundTop");
	NF_CreateTiledBg(1, 3, "backgroundBot");
	NF_CreateTextLayer16(0, 0, 0, "font");
}