#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "air_enemy.h"
#include "../parachuter/parachuter.h"

#define HFRAME 3

class Helicopter : public AirEnemy
{

public:
	Helicopter();
	Helicopter(int cX, int cY, int speed, int pcount);
	~Helicopter();

	// these methods are overloaded from AirEnemy
	void draw(SDL_Surface *screen);
	void move();

	void setHelicopter(int cX, int cY, int speed, int pcount);
	int getParachuterCount();
	Parachuter* getParachuters();

private:
	void jump(int);
	void x_simetri();
	void randomize_wheretojump(int);
	SDL_Surface *helSprites[HFRAME];
	SDL_Surface *explosionSprite;
	SDL_Rect expRect;
	int animationTime;
	int frameIndex;
	int frameTime;
	int speed;
	bool notPlayedYet; // For helicopter music.
	static const int frameDelay = 20; // Delay between helicopter frames
	Parachuter *parachuters;
	int parachuterCount;
	int *whereToJump;
	Mix_Chunk *helicopterSfx;
};

#endif
