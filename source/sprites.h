#ifndef SPRITES_H
#define SPRITES_H

#include <nf_lib.h>

class Sprite {
	protected:
	int X;
	int Y;
	int frame;
	int ID;
	int screen;
	int gfx;
	int pal;
	//int vel;

	public:
	void setScreen(int setScreen);
	void setID(int setID);
	void setX(int setX);
	void setY(int setY);
	void setFrame(int setFrame);
	void setGfx(int setGfx);
	void setPal(int setPal);
	int getID();
	int getX();
	int getY();
	int getFrame();
	int getScreen();
	/*
	void velSwitch();
	void velLeft();
	void velRight();
	*/
	void create();
	void updatePos();
	void show(bool setShow);
	void setPos(int setX, int setY);
	void move(int setX, int setY);
	void del();
};

#endif