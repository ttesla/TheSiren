#include "hamur.h"


// STATIC DEGISKENLER
int Hamur::SCREEN_WIDTH	 		= 0;
int Hamur::SCREEN_HEIGHT	 	= 0;
int Hamur::SCREEN_BPP		 	= 0;
int Hamur::FRAMES_PER_SECOND 	= 0;
SDL_Surface *Hamur::SCREEN		= NULL;



// LOAD IMAGE -- WITH COLOR KEY
SDL_Surface* Hamur::load_image( string filename, int red, int green, int blue )
{
	SDL_Surface* loadedImage	= NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() ); // Load Image

    if( loadedImage != NULL )
	{
		/////////// COLOR KEYING ///////////
		Uint32 colorkey = SDL_MapRGB( loadedImage->format, red, green, blue );
		SDL_SetColorKey( loadedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
		////////////////////////////////////

		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
    }
	else
	{
		error_message_handler("Image load error!  File: " + filename); // error msg.
		exit(1);
	}

    return optimizedImage;
}


// LOAD IMAGE -- PLAIN
SDL_Surface* Hamur::load_image( string filename )
{
	SDL_Surface *loadedImage	= NULL;
    SDL_Surface *optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() ); // Load Image

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
	else
	{
		error_message_handler("Image load error!  File: " + filename); // error msg.
		exit(1);
	}

    return optimizedImage;
}


// LOAD MUSIC
Mix_Music* Hamur::load_music(string filename)
{
	Mix_Music *loadedMusic = NULL;
	loadedMusic = Mix_LoadMUS(filename.c_str()); // Load music

	if(loadedMusic == NULL)
	{
		error_message_handler("Music load error! File: " + filename);
		exit(1);
	}

	return loadedMusic;
}


// LOAD SOUND EFFECT - CHUNK
Mix_Chunk* Hamur::load_soundfx(string filename)
{
	Mix_Chunk *loadedSoundfx = NULL;
	loadedSoundfx = Mix_LoadWAV(filename.c_str()); // Load soundfx

	if(loadedSoundfx == NULL)
	{
		error_message_handler("Soundfx load error! File: " + filename);
		exit(1);
	}

	return loadedSoundfx;
}


// PRINTS OUT ERROR MESSAGES - cout version ?
void Hamur::error_message_handler(string error_message)
{
	fprintf(stderr, "%s\n", error_message.c_str());
}


// PRINTS OUT MESSAGES -  cout version ?
void Hamur::message_handler(string message)
{
	fprintf(stdout, "%s\n", message.c_str());
}


// PRINT SURFACE -- WITH RECT SUPPORT
void Hamur::draw_surface( int x, int y, SDL_Surface* source, SDL_Surface* target, SDL_Rect *rect)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, rect, target, &offset );
}

// PRINT SURFACE ONCE -- MEMORY SAVER -- WITH RECT SUPPORT
// TTF font icin yapildi, baska bir sey basmak icin kullanilirsa sorun yaratabilir.
void Hamur::draw_surface_once( int x, int y, SDL_Surface* source, SDL_Surface* target, SDL_Rect *rect)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, rect, target, &offset );
    SDL_FreeSurface(source); // Free the source to regain memory
}


// PRINT SURFACE -- PLAIN
void Hamur::draw_surface( int x, int y, SDL_Surface* source, SDL_Surface* target)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, NULL, target, &offset );
}


// PRINT SURFACE ONCE -- MEMORY SAVER -- PLAIN
void Hamur::draw_surface_once( int x, int y, SDL_Surface* source, SDL_Surface* target)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, NULL, target, &offset );
	SDL_FreeSurface(source); // Free the source to regain memory
}

// INITIALIZE SDL WITH SCREEN OPTIONS
// See SDL Video documentation for flags
/*	- SDL_DOUBLEBUF
	- SDL_FULLSCREEN
	- SDL_NOFRAME

	init edilenler...
	SDL_INIT_TIMER
    SDL_INIT_AUDIO
    SDL_INIT_VIDEO
    SDL_INIT_CDROM
    SDL_INIT_JOYSTICK
    SDL_Mixer
	TTF_FONT

*/
bool Hamur::init(string caption_name, int scr_width, int scr_height, int scr_bpp, int fps, Uint32 flags)
{
	SCREEN_WIDTH		= scr_width;
	SCREEN_HEIGHT		= scr_height;
	SCREEN_BPP			= scr_bpp;
	FRAMES_PER_SECOND	= fps;

    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
		error_message_handler(SDL_GetError());
        return false;
    }

	SCREEN = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags);

    if( SCREEN == NULL )
    {
		error_message_handler(SDL_GetError());
        return false;
    }

	//Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 )
    {
		error_message_handler(SDL_GetError());
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        error_message_handler(TTF_GetError());
        return false;
    }

    SDL_WM_SetCaption( caption_name.c_str(), NULL );

    return true; //BINGO!
}


// GET SCREEN
SDL_Surface* Hamur::getScreen()
{
	return SCREEN;
}


// GET FPS CONSTANT
int Hamur::getFPSConstant()
{
	return FRAMES_PER_SECOND;
}

// GET SCREEN_BPP
int Hamur::getScreen_BPP()
{
	return SCREEN_BPP;
}

// GET SCREEN WIDTH
int Hamur::getScreen_WIDTH()
{
	return SCREEN_WIDTH;
}

// GET SCREEN HEIGHT
int Hamur::getScreen_HEIGHT()
{
	return SCREEN_HEIGHT;
}

// QUIT SDL - HAMUR
void Hamur::hamur_quit()
{
	SDL_FreeSurface(SCREEN);
    Mix_CloseAudio();
    TTF_Quit();
	SDL_Quit();
}


// QUIT SDL and RETURN TO GP2X MENU
void Hamur::hamur_quit_gp2x()
{
	SDL_FreeSurface(SCREEN);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    chdir("/usr/gp2x");
    execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
}


// RECTANGULAR COLLISION
bool Hamur::check_collision(const SDL_Rect &A, const SDL_Rect &B)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}



/*
 * Taken from the SDL documentation...
 *
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 Hamur::getpixel(SDL_Surface *surface, int x, int y)
{
	// if it is out of video memory
	if( x<0 || x >= surface->w || y<0 || y >= surface->h)
		return 0;

    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


/*
 * Taken from the SDL documentation...
 *
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void Hamur::putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	// if it is out of video memory
	if( x<0 || x >= surface->w || y<0 || y >= surface->h)
		return;

    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


// LOCK SCREEN - SURFACE for direct access
void Hamur::lock_surface(SDL_Surface *surface)
{
	if ( SDL_MUSTLOCK(surface) )
	{
		if ( SDL_LockSurface(surface) < 0 )
		{
			error_message_handler(SDL_GetError());
			exit(1);
		}
	}
}


// UNLOCK SCREEN - SURFACE for direct access
void Hamur::unlock_surface(SDL_Surface *surface)
{
	if( SDL_MUSTLOCK(surface) )
	{
		SDL_UnlockSurface(surface);
	}
}


// SET ALPHA - TRANSPARENCY
void Hamur::set_alpha(SDL_Surface *surface, int alphaValue)
{
	if(alphaValue < 0) alphaValue = 0;
	else if(alphaValue > 255) alphaValue = 255;

	SDL_SetAlpha(surface, SDL_SRCALPHA | SDL_RLEACCEL, alphaValue);
}


// DRAW CIRCLE
void Hamur::draw_circle(SDL_Surface *surface, int x, int y, int radius, Uint32 color)
{
	float deg = 0;

    do
	{
		putpixel(surface, (int)(x + radius * cos(deg)), (int)(y + radius * sin(deg)), color);
        deg += 0.005f; // sensitivity
    }
    while (deg <= 6.4);
}


// FILL CIRCLE -- SUPERB!!!
void Hamur::fill_circle(SDL_Surface *surface, int x, int y, int radius, Uint32 color)
{
    int nz;
    int hr = radius / 2;

    for(int i = 1; i < radius; ++i)
        for(int j = 1; j < radius; ++j)
        {
           nz =  (i - hr) * (i - hr) + (j - hr) * (j - hr);

           if( (nz / hr) < hr) putpixel(surface, x + i, y + j, color);
        }
}


// FONT YUKLE
TTF_Font* Hamur::load_font(string file_name, int size)
{
    TTF_Font *font = NULL;
    font = TTF_OpenFont( file_name.c_str(), size );

    if(font == NULL)
    {
        error_message_handler("Font load failed! : " + file_name);
        exit(1);
    }

    return font;
}


// JOYSTICK AC
SDL_Joystick* Hamur::open_joystick()
{
	SDL_Joystick *joy = NULL;

	if (SDL_NumJoysticks() > 0)
	{
		joy = SDL_JoystickOpen(0);

		if(!joy)
		{
			Hamur::error_message_handler("Couldn't open joystick");
			exit(1);
		}
	}

	return joy;
}

// Bresenham Draw Line
// Taken from a mail list thread
void Hamur::draw_line(SDL_Surface *surface, int x1, int y1, int x2, int y2,
						Uint32 color)
{
	int lg_delta, sh_delta, cycle, lg_step, sh_step;

  	lg_delta = x2 - x1;
  	sh_delta = y2 - y1;
  	lg_step  = SGN(lg_delta);
  	lg_delta = ABS(lg_delta);
  	sh_step  = SGN(sh_delta);
  	sh_delta = ABS(sh_delta);

	if (sh_delta < lg_delta)
	{
    	cycle = lg_delta >> 1;

		while (x1 != x2)
		{
      		putpixel(surface, x1, y1, color);
		    cycle += sh_delta;

	  		if (cycle > lg_delta)
			{
				cycle -= lg_delta;
				y1 += sh_step;
      		}

      		x1 += lg_step;
    	}
    	putpixel(surface, x1, y1, color);
  	}

  	cycle = sh_delta >> 1;

  	while (y1 != y2)
  	{
    	putpixel(surface, x1, y1, color);
    	cycle += lg_delta;

    	if (cycle > sh_delta)
		{
    		cycle -= sh_delta;
    	 	 x1 += lg_step;
    	}
	    y1 += sh_step;
	}

  	putpixel(surface, x1, y1, color);
}

// Rotates source image and blits it over target surface...
// target source and source surfaces may have different sizes.
// This one uses radian degree form
void Hamur::rotate(int corX, int corY, int refX, int refY, float degree,
	SDL_Surface *source, SDL_Surface *target)
{
    int x0 = refX;
	int y0 = refY;

    Hamur::lock_surface(source); // pixel operations
    Hamur::lock_surface(target); // pixel operations

    for(int y = 0 ; y < source->h ; y++)
    for(int x = 0 ; x < source->w ; x++)
    {
		float x1 = (float) (x - x0); // Translate
        float y1 = (float) (y - y0);

        float rotx = x1*cos(degree) - y1*sin(degree); // Rotate
        float roty = x1*sin(degree) + y1*cos(degree);

    	rotx += x0; // Untranslate
	    roty += y0;

		Uint32 pix = Hamur::getpixel(source, x,y);
        Hamur::putpixel(target, (int)(corX + rotx), (int)(corY + roty), pix);
	}

    Hamur::unlock_surface(target);
    Hamur::unlock_surface(source);
}

// Rotates source image and blits it over target surface...
// target source and source surfaces may have different sizes.
// This one uses normal 360 degree form
void Hamur::rotate(int corX, int corY, int refX, int refY, int deg360,
	SDL_Surface *source, SDL_Surface *target)
{
    int x0 = refX;
	int y0 = refY;
	float degree = PI * deg360 / 180.0f;

    Hamur::lock_surface(source); // pixel operations
    Hamur::lock_surface(target); // pixel operations

    for(int y = 0 ; y < source->h ; y++)
    for(int x = 0 ; x < source->w ; x++)
    {
		float x1 = (float) (x - x0); // Translate
        float y1 = (float) (y - y0);

        float rotx = x1*cos(degree) - y1*sin(degree); // Rotate
        float roty = x1*sin(degree) + y1*cos(degree);

    	rotx += x0; // Untranslate
	    roty += y0;

		Uint32 pix = Hamur::getpixel(source, x,y);
        Hamur::putpixel(target, (int)(corX + rotx), (int)(corY + roty), pix);
	}

    Hamur::unlock_surface(target);
    Hamur::unlock_surface(source);
}



// Rotates source image and blits it over target surface...
// target source and source surfaces may have different sizes.
// This one uses radian degree form
// + Colorkey support
void Hamur::rotate(int corX, int corY, int refX, int refY, float degree,
	SDL_Surface *source, SDL_Surface *target, Uint32 colorkey)
{
    int x0 = refX;
	int y0 = refY;

    Hamur::lock_surface(source); // pixel operations
    Hamur::lock_surface(target); // pixel operations

    for(int y = 0 ; y < source->h ; y++)
    for(int x = 0 ; x < source->w ; x++)
    {
		float x1 = (float) (x - x0); // Translate
        float y1 = (float) (y - y0);

        float rotx = x1*cos(degree) - y1*sin(degree); // Rotate
        float roty = x1*sin(degree) + y1*cos(degree);

    	rotx += x0; // Untranslate
	    roty += y0;

		Uint32 pix = Hamur::getpixel(source, x,y);

		if(pix != colorkey)
	        Hamur::putpixel(target, (int)(corX + rotx), (int)(corY + roty), pix);
	}

    Hamur::unlock_surface(target);
    Hamur::unlock_surface(source);
}

// Rotates source image and blits it over target surface...
// target source and source surfaces may have different sizes.
// This one uses normal 360 degree form
// + Colorkey support
void Hamur::rotate(int corX, int corY, int refX, int refY, int deg360,
	SDL_Surface *source, SDL_Surface *target, Uint32 colorkey)
{
    int x0 = refX;
	int y0 = refY;
	float degree = PI * deg360 / 180.0f;

    Hamur::lock_surface(source); // pixel operations
    Hamur::lock_surface(target); // pixel operations

    for(int y = 0 ; y < source->h ; y++)
    for(int x = 0 ; x < source->w ; x++)
    {
		float x1 = (float) (x - x0); // Translate
        float y1 = (float) (y - y0);

        float rotx = x1*cos(degree) - y1*sin(degree); // Rotate
        float roty = x1*sin(degree) + y1*cos(degree);

    	rotx += x0; // Untranslate
	    roty += y0;

		Uint32 pix = Hamur::getpixel(source, x,y);

		if(pix != colorkey)
	        Hamur::putpixel(target, (int)(corX + rotx), (int)(corY + roty), pix);
	}

    Hamur::unlock_surface(target);
    Hamur::unlock_surface(source);
}


// GET SIMETRI TO X
void Hamur::x_simetri(SDL_Surface *tsurface)
{
	Uint32 pix1, pix2;
	int width  = tsurface->w;
	int height = tsurface->h;

	Hamur::lock_surface(tsurface);

	for(int y = 0; y < height; ++y)
	for(int x = 0; x < width/2; ++x)
	{
		pix1 = Hamur::getpixel(tsurface, x, y);
		pix2 = Hamur::getpixel(tsurface,  width - x - 1, y);

		Hamur::putpixel(tsurface, x, y, pix2);
		Hamur::putpixel(tsurface, width - x - 1, y, pix1);
	}

	Hamur::unlock_surface(tsurface);
}

// GENERATES RAMDOM NUMBERS BETWEEN "BOTTOM" and "TOP"
int Hamur::random_generator(int bottom, int top)
{
	return rand()%top + bottom;
}


// FILL GIVEN RECTANGULAR AREA WITH THE GIVEN COLOR
void Hamur::fill_rect(SDL_Surface *surface, int x, int y, int w, int h, int r, int g, int b)
{
	SDL_Rect tempRect;
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;

	SDL_FillRect(surface, &tempRect, SDL_MapRGB( surface->format, r, g, b));
}
