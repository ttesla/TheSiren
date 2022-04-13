#ifndef BULLETFLAK_H
#define BULLETFLAK_H

#include "../hamur/hamur.h"
#include "bullet.h"


#define COLOR_RANGE 120

class BulletFlak : public Bullet
{

public:
	BulletFlak();
	~BulletFlak();
	
	void setfBullet(int sx,int sy,float deg);
	void move();
	void draw(SDL_Surface *screen);
	
    	
private:
	SDL_Surface *tempSurface;
	SDL_Surface *bulletExplode;
	SDL_Rect expRect;
	int animationTime;
	Uint32 colorkey;
	int cx1, cx2, cx3;
	int cy1, cy2, cy3;
	Uint32 colorTable[COLOR_RANGE]; // for making bullet red slowly
	int colorIndex;                 //  "    "       "    "    "
};


#endif
