#ifndef TUFEK_H
#define TUFEK_H

#include "../hamur/hamur.h"
#include "../bullet/bulletFlak.h"

#define BCOUNT 30
#define MAX_TUFEK 3

class Tufek
{

public:
	Tufek();
	~Tufek();
	void tufekMove(int mouseX, int mouseY); // mouse move
	void draw(SDL_Surface *screen);
	void fire();
	void delay();
	bool is_hit();
	void reset(); // To call tufek back from graveyard.
	void delay_continue();
	float getDegree();
	BulletFlak* get_bullets();
	int getBulletCount();
	void hit();

	SDL_Rect get_rect();

private:

 	//void draw_health(SDL_Surface *);
 	void calculate_uc();
	SDL_Surface *tufekSprite;
	SDL_Surface *tufekSpriteAll[MAX_TUFEK];
	SDL_Surface *bunker;
	SDL_Surface *targetCross;
	SDL_Surface *tempSurface;
	SDL_Surface *tempSurface2;
	SDL_Surface *temperMeter;
	SDL_Surface *explosionSprite;

	SDL_Rect expRect;
	SDL_Rect tufekRect;
	int animationTime;
	int tufekRoller;

	Mix_Chunk *explodeSfx;
	Mix_Chunk *gunSfx;
	Mix_Chunk *overHeatSfx;

	BulletFlak bullets[BCOUNT];

	int corX;
	int corY;
	int midX;
	int midY;
	int ucX;
	int ucY;
	int mouseX;
	int mouseY;
	int bulletCount;
	int bunkX;
	int bunkY;
	bool isDelay;
	bool isSet;
	bool isHit;
	int delayCount;
	float degree;
	Uint32 colorkey;
	Uint32 temperColor;

};


#endif
