#ifndef BOMBER_H
#define BOMBER_H

#include "../hamur/hamur.h"
#include "air_enemy.h"
#include "../bullet/bulletBomber.h"


class Bomber : public AirEnemy
{

public:
	Bomber();
	Bomber(int cX, int cY,int tarX,int tarY,int Velocity,int bX);
	~Bomber();

	void setBomber(int corX, int corY,int tarX,int tarY,int Velocity,int bomX);
	BulletBomber* getBomberBullet();

	void move();
	void draw(SDL_Surface *screen);
	void fire();


private:
	SDL_Surface *bomberSprite;
	SDL_Surface *explosionSprite;
	Mix_Chunk *bomberSfx;

	SDL_Rect expRect;

	int animationTime;

	int Velocity;
	int targetX;
	int targetY;
	int bomX;
	float degree;
	BulletBomber bullet;
	bool notPlayedYet;

	void x_simetri();

};


#endif
