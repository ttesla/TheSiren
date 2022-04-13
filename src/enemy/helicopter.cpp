#include "helicopter.h"

Helicopter::Helicopter()
{
	helSprites[0] = Hamur::load_image("res/gfx/helicopter0.png", 255, 0, 255);
	helSprites[1] = Hamur::load_image("res/gfx/helicopter1.png", 255, 0, 255);
	helSprites[2] = Hamur::load_image("res/gfx/helicopter2.png", 255, 0, 255);

	frameIndex = 0;
	frameTime = 0;
	health = 4; // 3 hit blows helicopter
	notPlayedYet = true;
	srand(time(NULL) + rand()); // seed rand()

	animationTime = 0;
	expRect.x = 0;
	expRect.y = 0;
	expRect.w = 127;
	expRect.h = 116;

}

Helicopter::Helicopter(int cX, int cY, int speed, int pcount)
{
	helSprites[0] = Hamur::load_image("res/gfx/helicopter0.png", 255, 0, 255);
	helSprites[1] = Hamur::load_image("res/gfx/helicopter1.png", 255, 0, 255);
	helSprites[2] = Hamur::load_image("res/gfx/helicopter2.png", 255, 0, 255);

	explosionSprite = Hamur::load_image("res/gfx/explosion2.png", 255, 0, 255);
	helicopterSfx = Hamur::Hamur::load_soundfx("res/sfx/helicopter.wav");

	frameIndex = 0;
	frameTime = 0;
	health = 4; // 3 hit blows helicopter
	notPlayedYet = true;
	srand(time(NULL) + rand()); // seed rand()

	setHelicopter(cX, cY, speed, pcount);

	animationTime = 0;
	expRect.x = 0;
	expRect.y = 0;
	expRect.w = 127;
	expRect.h = 116;
}

Helicopter::~Helicopter()
{
	for(int i = 0; i < HFRAME; ++i)
		SDL_FreeSurface(helSprites[i]);

  SDL_FreeSurface(explosionSprite);
  Mix_FreeChunk(helicopterSfx);

	delete[] parachuters;
	delete[] whereToJump;
}

void Helicopter::setHelicopter(int cX, int cY, int spd, int pcount)
{
	// Arrange collusion coordinates
	corX = cX;
	corY = cY;
	hitY = corY;
	enemyRect.w = helSprites[0]->w - 20; // resim collusion a uysun diye
    enemyRect.h = helSprites[0]->h - 15;
	notPlayedYet = true;
	speed = spd;

	if(speed < 0) x_simetri(); // reverse the direction

	parachuterCount = pcount;
	parachuters		= new Parachuter[pcount];
	randomize_wheretojump(pcount); // generate random jumping points
	isSet = true;
}


// move of helicopter
void Helicopter::move()
{
	if(isSet && !isHit)
	{
		if(notPlayedYet)
		{
			Mix_PlayChannel(-1, helicopterSfx, 2);
			notPlayedYet = false;
		}

		corX += speed;
		hitX = corX;

		if(speed > 0 && corX > Hamur::getScreen_WIDTH())
			isSet = false;
		else if( speed<0 && corX < -helSprites[0]->w)
			isSet = false;

		// burasi parachute sayisina gore random olcak, zaten oyle ;)
		for(int i = 0; i < parachuterCount; ++i)
		{
			if( corX <= (whereToJump[i] + ABS(speed/2)) &&
				corX >= (whereToJump[i] - ABS(speed/2))    )
				  jump(i);
		}
	}
}


//parachuter jumps here
void Helicopter::jump(int i)
{
	// parachutcunun ozellikleri, parachute acacagi zaman random
	parachuters[i].setParachuter(corX, corY, 4, 2, rand() % 360);
}

// classic draw method
void Helicopter::draw(SDL_Surface *screen)
{

	if(isSet && !isHit)
	{
		Hamur::draw_surface(corX, corY, helSprites[frameIndex%HFRAME], screen);

		if(SDL_GetTicks() - frameTime > frameDelay)
		{
			frameIndex++;
			frameTime = SDL_GetTicks();
		}
	}
	else if(isSet && isHit)
	{
		Hamur::draw_surface(corX - 19, corY - 40, explosionSprite, screen, &expRect);

		if(SDL_GetTicks() - animationTime > 75)
		{
			animationTime = SDL_GetTicks();
			expRect.x += 127;

			if(expRect.x > 1016)
			{
				isSet = false;
				expRect.x = 0;
			}
		}
	}


	// Parachute
	for(int i = 0; i < parachuterCount; ++i)
	{
		parachuters[i].move();
		parachuters[i].draw(screen);
	}
}

// If speed is negative, get sytmetri to X
void Helicopter::x_simetri()
{
	Uint32 pix1, pix2;
	int width  = helSprites[0]->w;
	int height = helSprites[0]->h;

	for(int i = 0; i < HFRAME; ++i)
	{
		Hamur::lock_surface(helSprites[i]);

		for(int y = 0; y < height; ++y)
		for(int x = 0; x < width/2; ++x)
		{
			pix1 = Hamur::getpixel(helSprites[i], x, y);
			pix2 = Hamur::getpixel(helSprites[i],  width - x - 1, y);

			Hamur::putpixel(helSprites[i], x, y, pix2);
			Hamur::putpixel(helSprites[i], width - x - 1, y, pix1);
		}

		Hamur::unlock_surface(helSprites[i]);
	}
}


// Generates random jumping coordinates.
void Helicopter::randomize_wheretojump(int pCount)
{
	whereToJump = new int[pCount];

	for(int i = 0; i < pCount; ++i)
	{
		int randomX = rand() % (Hamur::Hamur::getScreen_WIDTH() - 40);

		if(randomX < 521 && randomX > 240)
		{
			i--;
			continue;
		}

		whereToJump[i] = randomX;
	}
}


// get parachuters in Helicopter
Parachuter* Helicopter::getParachuters()
{
	return parachuters;
}

int Helicopter::getParachuterCount()
{
	return parachuterCount;
}
