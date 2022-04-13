#ifndef PARACHUTER_H
#define PARACHUTER_H

#include "../hamur/hamur.h"


class Parachuter
{

public:
	Parachuter();
	~Parachuter();
	void setParachuter(int corX, int corY,int cVelocity,int oVelocity,int openY);
	void setCorX(int cx) { corX = cx; }
	void setCorY(int cy) { corY = cy; }
	bool is_open();
	bool is_ground();
	bool is_set();
	void kill();
	void man_hit();
	void parac_hit();

	void move();
	void draw(SDL_Surface *screen);

	SDL_Rect get_man_rect();
	SDL_Rect get_parachute_rect();

private:
	SDL_Surface *pOpenSprite;
	SDL_Surface *pCloseSprite;
	SDL_Surface *pGroundSprite;
	SDL_Surface *manExplode;
	SDL_Rect expRect;
	Mix_Chunk *death1Sfx;
	Mix_Chunk *death2Sfx;
	Mix_Chunk *death3Sfx;

	int oVelocity;
	int cVelocity;
	int openY;
	int animationTime;

	bool isOpen;
	bool isGround;
	bool isManHit;
	bool isParacHit;
	int depth;

	int corX;
	int corY;
	bool isSet;

};


#endif
