#ifndef BULLETBOMBER_H
#define BULLETBOMBER_H

#include "../hamur/hamur.h"
#include "bullet.h"


class BulletBomber : public Bullet
{

public:
	BulletBomber();
	~BulletBomber();

	void setbBullet(int sx,int sy,float deg);
	void move();
	void draw(SDL_Surface *screen);
	void rotate_bomb(float degree);


private:

	SDL_Surface *tempSurface;
	SDL_Surface *explodeSurface;
	Mix_Chunk *explodeSfx;
	int animationTime;
	SDL_Rect expRect;
	Uint32 colorkey;
    bool explodeOnce;
};


#endif
