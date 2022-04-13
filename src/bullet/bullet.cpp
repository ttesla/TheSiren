#include "bullet.h"

Bullet::Bullet()
{

   corY = 0;
   corX = 0;
   distance = 0;
   isSet = false;
   isHit = false;
}

SDL_Rect Bullet::getRect()
{
	if(isSet && !isHit)
	{
		bulletRect.x = hitX;
		bulletRect.y = hitY;
	}

	else
	{
		bulletRect.x = 0;
		bulletRect.y = 0;
		bulletRect.w = 0;
		bulletRect.h = 0;
	}

	return bulletRect;
}


bool Bullet::is_set()
{
	return isSet;
}

void Bullet::hit()
{
	isHit = true;
}

/*void Bullet::move()
{

}

void Bullet::draw(SDL_Surface *screen)
{

}*/


