#ifndef SETUP_H
#define SETUP_H
#include "sprites.h"

void loadGraphics();
void displayBackgrounds();
void spawnBuckets(Bucket bucket[], int bucketTop);
void setup(MadBomber &bomber, Bucket bucket[], int bucketTop);


#endif