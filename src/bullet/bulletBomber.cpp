#include "bulletBomber.h"

BulletBomber::BulletBomber()
{
    bulletSprite = Hamur::load_image("res/gfx/bBullet.png", 255, 255, 255);
    tempSurface  = SDL_CreateRGBSurface(SDL_HWSURFACE, bulletSprite->h*2,
					bulletSprite->h*2, Hamur::getScreen_BPP(), 0, 0, 0, 0);
	explodeSurface = Hamur::load_image("res/gfx/explosion2.png", 255, 0, 255);
	colorkey = SDL_MapRGB(tempSurface->format, 255, 0, 255 );
	SDL_SetColorKey( tempSurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
	SDL_FillRect(tempSurface, NULL, colorkey);

	explodeSfx = Hamur::load_soundfx("res/sfx/explode1.wav");

    isSet = false;
    explodeOnce = true;

	animationTime = 0;
	expRect.x = 0;
	expRect.y = 0;
	expRect.w = 127;
	expRect.h = 116;
}

BulletBomber::~BulletBomber()
{
    SDL_FreeSurface(bulletSprite);
	SDL_FreeSurface(tempSurface);
	SDL_FreeSurface(explodeSurface);
	Mix_FreeChunk(explodeSfx);
}

void BulletBomber::setbBullet(int cx,int cy,float deg)
{
	rotate_bomb(deg); // rotate bomber image
	distance = 0;
	animationTime = 0;
	corX = cx;
	corY = cy;
	startX = cx;
	startY = cy;
	degree = deg;

	// Arrange coordinates for collusion
	bulletRect.w = bulletSprite->w;
	bulletRect.h = bulletSprite->w; // caution!
	isSet = true;
	explodeOnce = true;
}


void BulletBomber::move()
{
	if(isSet && !isHit)
	{
    	corY = (int)( startY - sin(PI/2 - degree) * distance);
		corX = (int)( startX + sin(degree) * distance);
		hitX = corX + bulletSprite->h - 4;
		hitY = corY + bulletSprite->h - 2;

		distance += 4;
	}
}

void BulletBomber::draw(SDL_Surface *screen)
{
	if(isSet && !isHit)
		Hamur::draw_surface(corX,corY,tempSurface,screen);

	else if(isSet && isHit)
	{
		// Very bad design of code caused this if block here.
		// This addon comment is written on 7 Dec. 2008
		if(explodeOnce)
		{
			// Explode SFX goes here
			Mix_PlayChannel(-1, explodeSfx, 0);
			explodeOnce = false;
		}

		Hamur::draw_surface(corX, corY, explodeSurface, screen, &expRect);

		if(SDL_GetTicks() - animationTime > 75)
		{
			animationTime = SDL_GetTicks();
			expRect.x += 127;

			if(expRect.x >= 1016)
			{
				isSet = false;
				expRect.x = 0; // Be very careful about this!
			}
		}
	}

	/*
	// Collusion area test code
	bulletRect.x = hitX;
	bulletRect.y = hitY;
	SDL_FillRect(screen, &bulletRect, 0);
	*/
}


void BulletBomber::rotate_bomb(float dgr)
{
	SDL_FillRect(tempSurface, NULL, colorkey);

	Hamur::rotate(bulletSprite->h + 2, bulletSprite->h + 2,
		0, 0, dgr, bulletSprite, tempSurface);
}
