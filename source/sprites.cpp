#include "sprites.h"


void Sprite::setScreen(int setScreen) {
	screen = setScreen;
}

void Sprite::setID(int setID) {
	ID = setID;
}

void Sprite::setX(int setX) {
	X = setX;
}

void Sprite::setY(int setY) {
	Y = setY;
}

void Sprite::setFrame(int setFrame) {
	frame = setFrame;
	NF_SpriteFrame(screen, ID, frame);
}

void Sprite::setGfx(int setGfx) {
	gfx = setGfx;
}

void Sprite::setPal(int setPal) {
	pal = setPal;
}

int Sprite::getScreen() {
	return screen;
}

int Sprite::getID() {
	return ID;
}

int Sprite::getX() {
	return X;
}

int Sprite::getY() {
	return Y;
}

int Sprite::getFrame() {
	return frame;
}
/*
void Sprite::velSwitch() {
	vel = vel * -1;
}

void Sprite::velLeft() {
	vel = abs(vel) * -1;
}

void Sprite::velRight() {
	vel = abs(vel);
}
*/
void Sprite::create() {
	NF_CreateSprite(screen, ID, gfx, pal, X , Y);
}

void Sprite::updatePos(){
	NF_MoveSprite(screen, ID, X, Y);
}

void Sprite::show(bool setShow) {
	NF_ShowSprite(screen, ID, setShow);
}

void Sprite::setPos(int setX, int setY) {
	X = setX;
	Y = setY;
	this->updatePos();
}

void Sprite::move(int setX, int setY) {
	X = X + setX;
	Y = Y + setY;
	this->updatePos();
}

void Sprite::del() {
	NF_DeleteSprite(screen, ID);
}
