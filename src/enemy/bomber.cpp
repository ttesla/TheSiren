#include "bomber.h"

Bomber::Bomber()
{
   bomberSprite = Hamur::load_image("res/gfx/bomber.png", 255, 0, 255);
   health = 3;
}

Bomber::Bomber(int cX, int cY,int tarX,int tarY,int Velocity,int bX)
{
	bomberSprite = Hamur::load_image("res/gfx/bomber.png", 255, 0, 255);
	explosionSprite = Hamur::load_image("res/gfx/explosion2.png", 255, 0, 255);
	bomberSfx = Hamur::load_soundfx("res/sfx/bomber.wav");
	health = 3;
	setBomber(cX, cY, tarX, tarY, Velocity, bX);

	animationTime = 0;
	expRect.x = 0;
	expRect.y = 0;
	expRect.w = 127;
	expRect.h = 116;
}

void Bomber::setBomber(int cX, int cY,int tarX,int tarY,int vel,int bX)
{
	// Arrange collusion coordinates
	corX = cX;
	corY = cY;
	hitY = cY;
	enemyRect.w = bomberSprite->w;
	enemyRect.h = bomberSprite->h - 10; // resim collusion a uysun diye

	targetX = tarX;
	targetY = tarY;
	isSet = true;
	notPlayedYet = true; // For the bomber sfx play
	bomX = bX;

	if(vel > 0) x_simetri(); // reverse the direction
	Velocity = vel;
}

Bomber::~Bomber()
{
	SDL_FreeSurface(bomberSprite);
	SDL_FreeSurface(explosionSprite);
	Mix_FreeChunk(bomberSfx);
}


void Bomber::fire()
{
	if(isSet)
	{
    	float yuk = targetY - corY;
		float gen = targetX - corX;

		if(gen == 0)
			degree = PI;

		else if( targetX > corX && targetY > corY) // 2st area
			degree = PI * (90 + atan(yuk/gen) * 180 / PI) / 180.0f;

		else if(targetX < corX && targetY > corY) // 3th area
			degree = PI * (270 + atan(yuk/gen) * 180 / PI) / 180.0f;

		bullet.setbBullet(corX,corY+bomberSprite->h,degree);
	}
}

void Bomber::move()
{
	if(isSet && !isHit )
	{
		// Play jet fighter - bomber sfx.
		if(notPlayedYet)
		{
			Mix_PlayChannel(-1, bomberSfx, 0);
			notPlayedYet = false;
		}

		// Calculate where to drop the bomb
 		if( corX <= (bomX + ABS(Velocity/2)) &&
				corX >= (bomX - ABS(Velocity/2)) )
	  		fire();

 	 	corX+=Velocity;
 	 	hitX = corX;
	}
}

void Bomber::draw(SDL_Surface *screen)
{
	if(isSet  && !isHit )
		Hamur::draw_surface(corX,corY,bomberSprite,screen);

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

	bullet.move();
	bullet.draw(screen);
}

// If speed is negative, get sytmetri to X
void Bomber::x_simetri()
{
	Uint32 pix1, pix2;
	int width  = bomberSprite->w;
	int height = bomberSprite->h;

	Hamur::lock_surface(bomberSprite);

	for(int y = 0; y < height; ++y)
	for(int x = 0; x < width/2; ++x)
	{
		pix1 = Hamur::getpixel(bomberSprite, x, y);
		pix2 = Hamur::getpixel(bomberSprite,  width - x - 1, y);

		Hamur::putpixel(bomberSprite, x, y, pix2);
		Hamur::putpixel(bomberSprite, width - x - 1, y, pix1);
	}

	Hamur::unlock_surface(bomberSprite);
}

// Get Bomber's bullet
BulletBomber* Bomber::getBomberBullet()
{
	return &bullet;
}

