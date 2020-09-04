#ifndef SETUP_H
#define SETUP_H
#include "sprites.h"

void loadGraphics();
void displayBackgrounds();
void spawnBuckets();
void setup();

struct RoundVar 
{
	int bomberVelX;
	int bombValue;
	int bombFrequency;
	int bombVelY;
	int bombTarget;
};

#endif