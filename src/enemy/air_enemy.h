#ifndef AIRENEMY_H
#define AIRENEMY_H

#include "../hamur/hamur.h"
#include "../parachuter/parachuter.h"
#include "../bullet/bulletBomber.h"


class AirEnemy
{

public:
	AirEnemy();
	~AirEnemy();
	virtual void draw(SDL_Surface *screen) = 0;
	virtual void move() = 0;
	virtual int getParachuterCount()       {return 0;}
	virtual Parachuter* getParachuters()   {return NULL;}
	virtual BulletBomber* getBomberBullet() {return NULL;}
	SDL_Rect getRect();
	bool is_set();
	void hit();

private:
	Mix_Chunk *explodeSfx;
	Mix_Chunk *puvvSfx;


protected:
	int corX;
	int corY;
	int hitX;
	int hitY;
	bool isSet;
	bool isHit;
	int health;
	SDL_Rect enemyRect;
};


#endif
