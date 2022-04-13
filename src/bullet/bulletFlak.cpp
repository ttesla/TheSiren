#include "bulletFlak.h"

BulletFlak::BulletFlak()
{
	bulletSprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 2,
					15, Hamur::getScreen_BPP(), 0, 0, 0, 0);

	tempSurface  = SDL_CreateRGBSurface(SDL_HWSURFACE, bulletSprite->h*3,
					bulletSprite->h*3, Hamur::getScreen_BPP(), 0, 0, 0, 0);

	bulletExplode = Hamur::load_image("res/gfx/explosion.png", 255, 0, 255);

	expRect.x = 0;
	expRect.y = 0;
	expRect.w = 8;
	expRect.h = 8;
	animationTime = 0;

	// here is our famous light-red color keys
	colorkey = SDL_MapRGB(tempSurface->format, 255, 0, 255);

	// set tempsurface's color key
	SDL_SetColorKey(tempSurface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);

	for(int i = 0; i < COLOR_RANGE; ++i)
	{
		colorTable[i] = SDL_MapRGB(bulletSprite->format, COLOR_RANGE*2 - i/2,
								   COLOR_RANGE*2 - i*2, 0);
	}

	isSet = false;
	isHit = false;
}

BulletFlak::~BulletFlak()
{
	SDL_FreeSurface(bulletSprite);
	SDL_FreeSurface(tempSurface);
	SDL_FreeSurface(bulletExplode);
}


void BulletFlak::setfBullet(int sx,int sy, float deg)
{
	SDL_FillRect(tempSurface, NULL, colorkey);
	colorIndex = 0;
	animationTime = 0;
	distance = 0;
	corX = sx;
	corY = sy;
	startX = sx;
	startY = sy;
	degree = deg;

	// mermilerin ozel durumlarindan dolayi kasitli olarak yapilmistir
	bulletRect.w = bulletSprite->w;
	bulletRect.h = bulletSprite->w; // Caution!

	isHit = false;
	isSet = true;
}

void BulletFlak::move()
{
	if(isSet && !isHit)
	{
    	corY = (int)( startY - sin(PI/2 - degree) * distance);
		corX = (int)( startX + sin(degree) * distance);

		// refresh hit coordinates
		hitX = corX;
		hitY = corY;

		// coordinates for blur effect
		cy1 = (int)( startY - sin(PI/2 - degree) * (distance-7));
		cx1 = (int)( startX + sin(degree) 		 * (distance-7));
		cy2 = (int)( startY - sin(PI/2 - degree) * (distance-15));
		cx2 = (int)( startX + sin(degree) 		 * (distance-15));
		cy3 = (int)( startY - sin(PI/2 - degree) * (distance-22));
		cx3 = (int)( startX + sin(degree) 		 * (distance-22));

		distance += 6;

		if(corX < -bulletSprite->w || corX > Hamur::getScreen_WIDTH() || corY < -bulletSprite->h)
			isSet = false;
	}
}

void BulletFlak::draw(SDL_Surface *screen)
{
	if(isSet && !isHit)
	{
		SDL_FillRect(bulletSprite, NULL, colorTable[colorIndex]);
		if(colorIndex < COLOR_RANGE - 1) colorIndex++;

		Hamur::rotate(tempSurface->w/2, 10,
			0, 0, degree, bulletSprite, tempSurface);

		Hamur::set_alpha(tempSurface, 255);// normal
		Hamur::draw_surface(corX - tempSurface->w/2, corY - 10, tempSurface, screen);
		Hamur::set_alpha(tempSurface, 80);// first alpha
		Hamur::draw_surface(cx1 - tempSurface->w/2, cy1 - 10, tempSurface, screen);
		Hamur::set_alpha(tempSurface, 50);// second alpha
		Hamur::draw_surface(cx2 - tempSurface->w/2, cy2 - 10, tempSurface, screen);
		Hamur::set_alpha(tempSurface, 30);// third alpha
		Hamur::draw_surface(cx3 - tempSurface->w/2, cy3 - 10, tempSurface, screen);
	}
	else if(isSet && isHit)
	{
		Hamur::draw_surface(corX, corY, bulletExplode, screen, &expRect);

		if(SDL_GetTicks() - animationTime > 50)
		{
			animationTime = SDL_GetTicks();
			expRect.x += 8;

			if(expRect.x > 56)
			{
				isSet = false;
				expRect.x = 0; // Caution!
			}
		}
	}
}
