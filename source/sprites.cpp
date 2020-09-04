#include "sprites.h"
#include <nf_lib.h>

void Sprite::setScreen(int screen) {
	screen_ = screen;
}

void Sprite::setID(int setID) {
	ID_ = setID;
}

void Sprite::setX(int setX) {
	X_ = setX;
}

void Sprite::setY(int setY) {
	Y_ = setY;
}

void Sprite::setFrame(int setFrame) {
	frame_ = setFrame;
	NF_SpriteFrame(screen_, ID_, frame_);
}

void Sprite::setGfx(int setGfx) {
	gfx_ = setGfx;
}

void Sprite::setPal(int setPal) {
	pal_ = setPal;
}

void Sprite::setLayer(int layer) {
    layer_ = layer;
    NF_SpriteLayer(screen_, ID_, layer_);
}

int Sprite::getScreen() {
	return screen_;
}

int Sprite::getID() {
	return ID_;
}

int Sprite::getX() {
	return X_;
}

int Sprite::getY() {
	return Y_;
}

int Sprite::getFrame() {
	return frame_;
}

bool Sprite::isSpawned() {
    return spawned;
}

void Sprite::create() {
	NF_CreateSprite(screen_, ID_, gfx_, pal_, X_ , Y_);
    spawned = true;
}

void Sprite::updatePos(){
	NF_MoveSprite(screen_, ID_, X_, Y_);
}

void Sprite::show(bool show) {
	NF_ShowSprite(screen_, ID_, show);
}

void Sprite::setPos(int X, int Y) {
	X_ = X;
	Y_ = Y;
	this->updatePos();
}

void Sprite::move(int X, int Y) {
	X_ = X_ + X;
	Y_ = Y_ + Y;
	this->updatePos();
}

void Sprite::del() {
    if (spawned)
    {
        NF_DeleteSprite(screen_, ID_);
        spawned = false;
    }
}



void MadBomber::velRev() {
    velX_ = velX_ * -1;
}

void MadBomber::velRight() {
    velX_ = abs(velX_);
}

void MadBomber::velLeft() {
    velX_ = abs(velX_) * -1;
}

void MadBomber::setVel(int vel) {
    velX_ = vel;
}

int MadBomber::getVel() {
    return velX_;
}

void MadBomber::screenBounce() {
    int r = rand()%200;
    if (X_ > 190 || X_ < 2 || r <2)
    {
    this->velRev();
    }
}


void Bucket::screenBind() {
    if (X_ < 0)
    {
        X_ = 0;
    } else if (X_ > 192)
    {
        X_ = 192;
    }
}

void Bucket::trackBucket(Bucket topBucket) {
    int topBucketX = topBucket.getX();
    X_ = topBucketX;
}

void Bucket::bucketScroll(int screenDis) {
    X_ = X_ + screenDis;
    this->screenBind();
    this->updatePos();
}


MadBomber bomber;
Bucket bucket[3];

void Bomb::spawn(int bombCount) {
    screen_ = 0;
    ID_ = bombCount + 4;
    X_ = bomber.getX()+22;
    Y_ = 100;
    NF_CreateSprite(0, ID_, 2, 2, X_, Y_);
    spawned = true;
}

void Bomb::bombScroll(int bombVelY) {
    move(0, bombVelY);
}

void Bomb::jumpScreen() {
    screen_ = 1;
    Y_ = -16;
    NF_CreateSprite(screen_, ID_, 2, 2, X_, Y_);
}

void Bomb::hide() {
    NF_ShowSprite(screen_, ID_, false);
    X_ = 350;
}

void Bomb::setFinal() {
    finalBomb_ = true;
}

void Bomb::clearFinal() {
    finalBomb_ = false;
}

bool Bomb::isFinal() {
    return finalBomb_;
}
