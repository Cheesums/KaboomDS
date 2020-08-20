#include "sprites.h"


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


void Sprite::create() {
	NF_CreateSprite(screen_, ID_, gfx_, pal_, X_ , Y_);
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
	NF_DeleteSprite(screen_, ID_);
}


void MadBomber::velRev() {
    vel_ = vel_ * -1;
}

void MadBomber::velRight() {
    vel_ = abs(vel_);
}

void MadBomber::velLeft() {
    vel_ = abs(vel_) * -1;
}

void MadBomber::setVel(int vel) {
    vel_ = vel;
}

int MadBomber::getVel() {
    return vel_;
}