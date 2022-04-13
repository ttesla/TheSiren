#include "tufek.h"
// 15 * 145

Tufek::Tufek()
{
	tufekSprite     = Hamur::load_image("res/gfx/minigun.png", 255, 0, 255);
	bunker	        = Hamur::load_image("res/gfx/base.png", 255, 0, 255);
	targetCross     = Hamur::load_image("res/gfx/targetCross.png", 255, 0, 255);
	temperMeter     = Hamur::load_image("res/gfx/temper.png");
	explosionSprite = Hamur::load_image("res/gfx/explosion2.png", 255, 0, 255);
	colorkey = SDL_MapRGB(tufekSprite->format, 255, 0, 255);

	explodeSfx  = Hamur::load_soundfx("res/sfx/explode1.wav");
	gunSfx      = Hamur::load_soundfx("res/sfx/mg.wav");
	overHeatSfx = Hamur::load_soundfx("res/sfx/heat.wav");

	tempSurface  = SDL_CreateRGBSurface(SDL_HWSURFACE, bunker->w + 10,
					15, Hamur::getScreen_BPP(), 0, 0, 0, 0);

	tempSurface2  = SDL_CreateRGBSurface(SDL_HWSURFACE, bunker->w + 10,
					15, Hamur::getScreen_BPP(), 0, 0, 0, 0);
	tempSurface2->w = 155;

	temperColor = SDL_MapRGB(tempSurface2->format, 240, 0, 0);
	SDL_FillRect(tempSurface2, NULL, temperColor);

	SDL_Rect tempRect;
	tempRect.x = 0;
	tempRect.y = 0;
	tempRect.w = tufekSprite->w / MAX_TUFEK;
	tempRect.h = tufekSprite->h;

	// Create surfaces for tufek animation.
	for(int i = 0; i < MAX_TUFEK; ++i)
	{
		tufekSpriteAll[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, tempRect.w,
					tempRect.h, Hamur::getScreen_BPP(), 0, 0, 0, 0);

		Hamur::draw_surface(0, 0, tufekSprite, tufekSpriteAll[i], &tempRect);

		tempRect.x += tempRect.w;
	}

	tempSurface->w = bulletCount;
	Hamur::set_alpha(tempSurface, 175);
	Hamur::set_alpha(tempSurface2, 200);

	corX = Hamur::getScreen_WIDTH()/2 - tufekSprite->w/3;
	corY = Hamur::getScreen_HEIGHT()*475/600;
	midX = corX + (tufekSprite->w)/3 - 5;
	midY = corY + tufekSprite->h;
	mouseX = Hamur::getScreen_WIDTH()/2;
	mouseY = Hamur::getScreen_HEIGHT()/2;
	bulletCount = 0;
	colorkey = SDL_MapRGB(tempSurface->format, 255, 0, 255 );
	isDelay = false;
	delayCount = 0;
	bunkX = 320;
	bunkY = 522;
	tufekRoller = 0;

	tufekRect.x = bunkX + 15;
	tufekRect.y = bunkY;
	tufekRect.w = 125;
	tufekRect.h = 76;

	animationTime = 0;
	expRect.x     = 0;
	expRect.y     = 0;
	expRect.w     = 127;
	expRect.h     = 116;

	isSet  = true;
	isHit  = false;
}


void Tufek::reset()
{
	isSet = true;
	isHit = false;
	isDelay = false;
	bulletCount = 0;
	delayCount = 0;

	// Reset - reset all bullets in the air
	for(int i = 0; i < BCOUNT; ++i)
		bullets[i].hit();
}

void Tufek::hit()
{
	Mix_PlayChannel(-1, explodeSfx, 0);
	Mix_PlayChannel(-1, explodeSfx, 0);
	isHit = true;
}


Tufek::~Tufek()
{
	SDL_FreeSurface(tufekSprite);
	SDL_FreeSurface(tufekSpriteAll[0]);
	SDL_FreeSurface(tufekSpriteAll[1]);
	SDL_FreeSurface(tufekSpriteAll[2]);
	SDL_FreeSurface(targetCross);
	SDL_FreeSurface(temperMeter);
	SDL_FreeSurface(explosionSprite);
	SDL_FreeSurface(bunker);
	SDL_FreeSurface(tempSurface);
	SDL_FreeSurface(tempSurface2);
	Mix_FreeChunk(explodeSfx);
	Mix_FreeChunk(gunSfx);
	Mix_FreeChunk(overHeatSfx);
}

// rotation with mouse
void Tufek::tufekMove(int mX, int mY)
{
	calculate_uc();
	mouseX = mX;
	mouseY = mY;

 	float yuk = mouseY - midY;
	float gen = mouseX - midX;

	if(gen == 0)
		degree = 0.0f;

	else if( mouseX > midX && mouseY < midY) // 1st area
		degree = PI * (90 + atan(yuk/gen) * 180 / PI) / 180.0f;

	else if(mouseX < midX && mouseY < midY) // 4th area
		degree = PI * (270 + atan(yuk/gen) * 180 / PI) / 180.0f;

	// Tufek kisitlamasi...
	int degNorm = (int)(degree * 180 / PI);

	if( degNorm <= 290 && degNorm >= 270)
		degree = PI * 290 / 180.0f;

	else if(degNorm <= 90 && degNorm >= 70)
		degree = PI * 70 / 180.0f;

}

float Tufek::getDegree()
{
      return degree;
}


void Tufek::calculate_uc()
{
	 ucY = (int)( midY - sin(PI/2 - degree) * tufekSprite->h);
	 ucX = (int)( midX + sin(degree) * tufekSprite->h);
}


void Tufek::fire()
{
	static int tufekCoolDown = 0;
	static int index = 0;

	if(SDL_GetTicks() - tufekCoolDown > 100 && !isDelay)
	{
		tufekCoolDown = SDL_GetTicks();
		calculate_uc();
		bullets[index % BCOUNT].setfBullet(ucX, ucY, degree);
		index++;
		bulletCount++;
		tufekRoller++;
		Mix_PlayChannel(-1, gunSfx, 0);
	}
}


void Tufek::delay()
{
	isDelay = true;
	Mix_PlayChannel(-1, overHeatSfx, 0);
}


void Tufek::delay_continue()
{
	isDelay = false;
}

SDL_Rect Tufek::get_rect()
{
	return tufekRect;
}

void Tufek::draw(SDL_Surface *screen)
{
	static int temp = 0;
	static int stime = 500;

	if(isSet && ! isHit)
	{
		temp = SDL_GetTicks() - stime;

		if ( temp > 400 )
		{
			if(isDelay)
			{
				delayCount++;
			}

			if ( bulletCount > 0 )
				bulletCount--;

			stime = SDL_GetTicks();
		}

		if( bulletCount > BCOUNT && isDelay == false )
		{
			delay();
		}

		if ( isDelay == true && delayCount > 6 )
		{
			delay_continue();
			delayCount = 0;
		}

		tempSurface->w = bulletCount*5;
		temperColor = SDL_MapRGB(tempSurface->format, tempSurface->w + 85, 200 - tempSurface->w, 0);
		SDL_FillRect(tempSurface, NULL, temperColor);

		for(int i = 0; i < BCOUNT; i++)
	    {
	    	bullets[i].move();
			bullets[i].draw(screen);
		}

		Hamur::rotate(corX,corY,(midX-corX),(midY-corY),degree,tufekSpriteAll[tufekRoller % MAX_TUFEK],screen,
			colorkey);

		Hamur::draw_surface(bunkX, bunkY, bunker, screen); // bunker(yuvarlak)
		Hamur::draw_surface(mouseX - targetCross->w/2, mouseY - targetCross->h/2,
							targetCross, screen);
	    Hamur::draw_surface(bunkX, bunkY + 60, temperMeter, screen);
		Hamur::draw_surface(bunkX, bunkY + 60, tempSurface, screen);

		static int showUp = 0;

		if(isDelay && showUp < 25)
		{
			Hamur::draw_surface(bunkX, bunkY + 60, tempSurface2, screen);
			showUp++;
		}

		else if(isDelay && showUp < 50) showUp++;
		else showUp = 0;

		static int tufekTimer =  0;

		if(SDL_GetTicks() - tufekTimer > 150)
		{
			tufekRoller++;
			tufekTimer = SDL_GetTicks();
		}
	}
	else if ( isSet && isHit )
	{
		Hamur::draw_surface(bunkX, bunkY, explosionSprite, screen, &expRect);
		Hamur::draw_surface(bunkX + 20, bunkY - 40, explosionSprite, screen, &expRect);
		Hamur::draw_surface(bunkX + 80, bunkY - 20, explosionSprite, screen, &expRect);
		Hamur::draw_surface(ucX - 50, ucY - 50, explosionSprite, screen, &expRect);
		Hamur::draw_surface(bunkX - 35, bunkY - 20, explosionSprite, screen, &expRect);

		if(SDL_GetTicks() - animationTime > 75)
		{
			animationTime = SDL_GetTicks();
			expRect.x += 127;

			if(expRect.x > 1016)
			{
				isSet = false;
				expRect.x = 0; // Never forget this again!
			}
		}
	}
}


// Return bullets in Tufek
BulletFlak* Tufek::get_bullets()
{
	return bullets;
}

int Tufek::getBulletCount()
{
	return BCOUNT;
}

bool Tufek::is_hit()
{
	return isHit;
}
