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

extern int fuseSound;
int catchSoundChannel;

extern Bucket bucket[3];
extern RoundVar roundVar[9];

void collision(Bomb &bomb) {

    int bucketTop = BUCKET_TOP + BUCKET_SPRITE_Y_OFFSET;
    int bucketBot = bucketTop + (remainingBuckets*BUCKET_OFFSET);

    int bucketX = bucket[0].getX() + BUCKET_SPRITE_X_OFFSET;
    int bombX = bomb.getX() + BOMB_SPRITE_X_OFFSET;
    int bombY = bomb.getY() + BOMB_SPRITE_Y_OFFSET;

    //check if bomb is withing X range of the buckets
    if ((bombX+BOMB_WIDTH >= bucketX) && (bombX <= bucketX+BUCKET_WIDTH))
    {
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
                    gameState = 2;
                    currentRound++;
                    if (currentRound > 8)
                    {
                        currentRound = 8;
                    }
                    
                }
                
            }
            
            
        }
    }
    
}