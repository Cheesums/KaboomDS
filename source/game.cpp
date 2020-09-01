#include "game.h"

extern int BUCKET_TOP;
extern int BUCKET_HEIGHT;
extern int BUCKET_WIDTH;
extern int BUCKET_OFFSET;

extern int BOMB_WIDTH;
extern int BOMB_HEIGHT;

extern int scoreInt;

extern Bucket bucket[3];

void collision(Bomb bomb) {

    int bucketBot = BUCKET_TOP + 3*BUCKET_OFFSET;

    int bucketX = bucket[0].getX();
    int bombX = bomb.getX();
    int bombY = bomb.getY();

    //check if bomb is withing X range of the buckets
    if ((bombX+BOMB_WIDTH >= bucketX) && (bombX <= bucketX+BUCKET_WIDTH))
    {
        for (int bucketTop = BUCKET_TOP; bucketTop < bucketBot; bucketTop = bucketTop + BUCKET_OFFSET)
        {
            if ((bombY+BOMB_HEIGHT >= bucketTop) && (bombY <= bucketTop+BUCKET_HEIGHT))
            {
                bomb.hide();
                scoreInt++;
            }
            
            
        }
    }
    
}