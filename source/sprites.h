#ifndef SPRITES_H
#define SPRITES_H

#include <nf_lib.h>

class Sprite {
	protected:
	int X_;
	int Y_;
	int frame_;
	int ID_;
	int screen_;
	int gfx_;
	int pal_;
	//int vel;

	public:
	void setScreen(int screen);
	void setID(int ID);
	void setX(int X);
	void setY(int Y);
	void setFrame(int frame);
	void setGfx(int gfx);
	void setPal(int pal);
	int getID();
	int getX();
	int getY();
	int getFrame();
	int getScreen();
	void create();
	void updatePos();
	void show(bool show);
	void setPos(int X, int Y);
	void move(int X, int Y);
	void del();
};

class MadBomber: public Sprite {
    protected:
    int vel_;

    public:
    void velRev();
    void velRight();
    void velLeft();
    void setVel(int vel);
    int getVel();
};

#endif