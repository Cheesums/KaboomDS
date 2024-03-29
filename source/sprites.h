#ifndef SPRITES_H
#define SPRITES_H

class Sprite {
	protected:
	int X_;
	int Y_;
	int frame_;
	int ID_ = 300; //initialize sprite ID outside the bounds of used sprites
	int screen_;
	int gfx_;
	int pal_;
    int layer_ = 0;
	bool spawned = false;

	public:
	void setScreen(int screen);
	void setID(int ID);
	void setX(int X);
	void setY(int Y);
	void setFrame(int frame);
	void setGfx(int gfx);
	void setPal(int pal);
    void setLayer(int layer);
	int getID();
	int getX();
	int getY();
	int getFrame();
	int getScreen();
	bool isSpawned();
	void create();
	void updatePos();
	void show(bool show);
	void setPos(int X, int Y);
	void move(int X, int Y);
	void del();
	void hide();
	void show();
};

class MadBomber: public Sprite {
    protected:
    int velX_;

    public:
    void velRev();
    void velRight();
    void velLeft();
    void setVel(int vel);
    int getVel();
    void bounce(int revFreq);
};

class Bucket: public Sprite {
    public:
    void screenBind();
    void trackBucket(Bucket bucketTop);
    void bucketScroll(int screenDis);
	int animateCatch(int frame);
};

class Bomb: public Sprite {
    protected:
    int velY_ = 2;
	bool finalBomb_ = false;

    public:
    void spawn(int bombCount);
    void bombScroll(int bombVelY);
    void jumpScreen();
	void setFinal();
	void clearFinal();
	bool isFinal();
	void cycleColor();
};


#endif