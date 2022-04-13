#include "air_enemy.h"

AirEnemy::AirEnemy()
{
	explodeSfx = Hamur::load_soundfx("res/sfx/explode1.wav");
	puvvSfx	   = Hamur::load_soundfx("res/sfx/puvv.wav"); 
	
	isSet = false;
	isHit = false;
    corY = 0;
    corX = 0;
}

AirEnemy::~AirEnemy()
{
	Mix_FreeChunk(explodeSfx);
	Mix_FreeChunk(puvvSfx);
}

bool AirEnemy::is_set()
{
	return isSet;
}

SDL_Rect AirEnemy::getRect()
{
	if(isSet && !isHit)
	{
		enemyRect.x = hitX;
		enemyRect.y = hitY;
	}
	else
	{
		enemyRect.x = 0;
		enemyRect.y = 0;
		enemyRect.w = 0;
		enemyRect.h = 0;
	}
	
	return enemyRect;
}


void AirEnemy::hit()
{
	Mix_PlayChannel(-1, puvvSfx, 0);
	health--;
	
	if(health <= 0)
	{
		Mix_PlayChannel(-1, explodeSfx, 0);
		isHit = true;
	}
}
