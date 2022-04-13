#ifndef BULLET_H
#define BULLET_H

#include "../hamur/hamur.h"


class Bullet
{

public:
	Bullet();

	bool is_set();

	//virtual void move();
	//virtual void draw(SDL_Surface *screen);
	SDL_Rect getRect();
	void hit();


private:

protected:

	int corX;
	int corY;
	int hitX; // for better collusion detection
	int hitY; //  "    "       "          "
	int startX;
	int startY;
	int distance;
	bool isSet;
	bool isHit;
    float degree;
    SDL_Rect bulletRect;
    SDL_Surface *bulletSprite;
};


#endif
