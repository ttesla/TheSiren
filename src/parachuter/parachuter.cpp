#include "parachuter.h"

Parachuter::Parachuter()
{
   pOpenSprite   = Hamur::load_image("res/gfx/pOpen.png",  255, 0, 255);
   pCloseSprite  = Hamur::load_image("res/gfx/pClose.png", 255, 0, 255); 
   pGroundSprite = Hamur::load_image("res/gfx/pGround.png",255, 0, 255);
   manExplode 	 = Hamur::load_image("res/gfx/blood.png" , 0, 255, 255);
   death1Sfx = Hamur::Hamur::load_soundfx("res/sfx/death1.wav");
   death2Sfx = Hamur::Hamur::load_soundfx("res/sfx/death2.wav");
   death3Sfx = Hamur::Hamur::load_soundfx("res/sfx/death3.wav");
   
   expRect.x = 0;
   expRect.y = 0;
   expRect.w = 57;
   expRect.h = 61;

   oVelocity = 0;
   cVelocity = 0;
   animationTime = 0;
   
   depth = rand()%45;
   
   isOpen = false;
   isGround = false;
   isSet = false;
   isManHit = false;
   isParacHit = false;
}

void Parachuter::setParachuter(int cX, int cY,int cV,int oV,int oY)
{
      corX = cX;
      corY = cY;
      cVelocity = cV;
      oVelocity = oV;
      openY = oY;
      isSet = true;
}


bool Parachuter::is_open()
{
	return isOpen;
}

bool Parachuter::is_ground()
{
	return isGround;
}

void Parachuter::kill()
{
	isSet = false;
}

Parachuter::~Parachuter()
{
    SDL_FreeSurface(pOpenSprite);
	SDL_FreeSurface(pCloseSprite);
	SDL_FreeSurface(pGroundSprite);
	SDL_FreeSurface(manExplode);
	Mix_FreeChunk(death1Sfx);
	Mix_FreeChunk(death2Sfx);  
	Mix_FreeChunk(death3Sfx);     
}

void Parachuter::move()
{
	if(isSet && !isManHit && ! isParacHit)
	{
    	if ( corY <= openY )
       		corY+=cVelocity;
     
    	else if( corY > openY )
    	{
       		isOpen = true;
       		corY+=oVelocity;
    	}
    
	    if ( corY >= Hamur::getScreen()->h - pCloseSprite->h - (depth) - 5)
	    {
	 		oVelocity = 0;
	 		isOpen = false;
	 		isGround = true;
	    }
	}
	else if(isSet && isParacHit)
	{
		corY += cVelocity;
	}
}

void Parachuter::draw(SDL_Surface *screen)
{
	if(isSet && !isManHit && ! isParacHit)
	{
 		if ( isOpen )
     		Hamur::draw_surface(corX,corY,pOpenSprite,screen);
     	
     	else if( isGround )
     		Hamur::draw_surface(corX,corY,pGroundSprite,screen);  
     		
     	else if( !isOpen ) // parachute is closed. Free fall
     		Hamur::draw_surface(corX,corY,pCloseSprite,screen);  
	}
	else if(isSet && isManHit)
	{
		Hamur::draw_surface(corX, corY, manExplode, screen, &expRect);
		
		if(SDL_GetTicks() - animationTime > 75)
		{
			animationTime = SDL_GetTicks();
			expRect.x += 57;
			
			if(expRect.x == 228)
				isSet = false;
		}
	}
	else if(isSet && isParacHit)
	{
		if(corY + pCloseSprite->h < Hamur::getScreen_HEIGHT())
			Hamur::draw_surface(corX,corY,pCloseSprite,screen);
		
		else
		{
			Hamur::draw_surface(corX, corY - 10, manExplode, screen, &expRect);
		
			if(SDL_GetTicks() - animationTime > 75)
			{
				animationTime = SDL_GetTicks();
				expRect.x += 57;
			
				if(expRect.x == 228)
					isSet = false;
			}
		}  
	}
}

bool Parachuter::is_set()
{
	return isSet;
}

SDL_Rect Parachuter::get_man_rect()
{
	SDL_Rect manRect;
	
	if(isSet && !isManHit)
	{
		manRect.x = corX;
		manRect.y = corY;
		manRect.w = pOpenSprite->w;
		manRect.h = pOpenSprite->h;
	
		if ( isOpen )
		{
			manRect.y = corY + 26;
			manRect.w = pOpenSprite->w;
			manRect.h = pOpenSprite->h-26;
		}
		else if ( !isOpen )
		{
			manRect.w = pCloseSprite->w;
			manRect.h = pCloseSprite->h;
		}
		else if( isGround)
		{
			manRect.y = corY;
			manRect.w = pCloseSprite->w;
			manRect.h = pCloseSprite->h;
		}
	}
	else
	{
		manRect.x = 0;
		manRect.y = 0;
		manRect.w = 0;
		manRect.h = 0;
	}
		
	return manRect;
}

SDL_Rect Parachuter::get_parachute_rect()
{
	SDL_Rect parachuteRect;
	
	if(isSet && !isManHit && !isParacHit)
	{
		parachuteRect.x = corX;
		parachuteRect.y = corY;
		
		if ( isOpen )
		{
			parachuteRect.w = pOpenSprite->w;
			parachuteRect.h = 26;
	    }
	    else
	    {
			parachuteRect.w = 0;
			parachuteRect.h = 0;
		}
	}
	else
	{
		parachuteRect.x = 0;
		parachuteRect.y = 0;
		parachuteRect.w = 0;
		parachuteRect.h = 0;	
	}
	
	return parachuteRect;
}

void Parachuter::man_hit()
{
	int ran = Hamur::random_generator(0,2);
	
	if(ran == 0)
		Mix_PlayChannel(-1, death1Sfx, 0);
	else
		Mix_PlayChannel(-1, death3Sfx, 0);
	
	isManHit = true;
}

void Parachuter::parac_hit()
{
	Mix_PlayChannel(-1, death2Sfx, 0);
	isParacHit = true;
}
                
