/**

  11.03.2008
  -----------
  * Colorkey ve SDL_DisplayFormat'in yerleri degistirildi.


  27.01.2007
  ----------
  * Hamur artik GP2X destekli


  xx.xx.2006
  ----------
  Bir somestir tatili(2006) hamur a baslandi. ve baslamadan bitti, 
  Sonradan Omer gp2x almasiyla bu yapboz hamur'u
  olusturdu. Gokten 3 elma dusmus, 3 u de bu kodu adam edicek insan evladina...

  SDL ile ilgilenen her Adem oglunun farkedecegi gibi bazi kesimlerde
  www.lazyfooproductions.com 'dan ilham alinmistir.

  sikayetler buraya :  omer.akyol@gmail.com



  -- bazý ise yarar foksiyonlar --

  SDL_ShowCursor(SDL_DISABLE); -- hide cursor - mouse


  ++ Surface
  SDL_Surface *SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);


  ++ TTF Font
  TTF_RenderText_Solid( font, "text", SDL_Color );

  ++ Pixel colors with alpha
  Uint32 SDL_MapRGB(screen->format, 255, 255, 0)
  Uint32 SDL_MapRGBA(SDL_PixelFormat *fmt, Uint8 r, Uint8 g, Uint8 b, Uint8 a)

  SDL_EnableKeyRepeat(1, 10) -- 1 delay at first, 10 delay between events.
  SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) ) -- Refresh screen

  ++ Play music usage ++

	Mix_Music   *music

	if( Mix_PlayMusic(music, -1) == -1 )
	{
		return 1;
	}
  ++

  Mix_FreeMusic(music)   -- Free Music
  SDL_FreeSurface(image) -- Free Surface


  --------------------------

**/

#ifndef HAMUR_H
#define HAMUR_H


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_thread.h>

#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h> // gp2x icin

#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0:(-1)))
#define ABS(x) ((x)>0 ? (x) : (-x))

#define PI 3.1415

using namespace std;

//GP2X key map
enum MAP_KEY
{
	VK_UP         , // 0
	VK_UP_LEFT    , // 1
	VK_LEFT       , // 2
	VK_DOWN_LEFT  , // 3
	VK_DOWN       , // 4
	VK_DOWN_RIGHT , // 5
	VK_RIGHT      , // 6
	VK_UP_RIGHT   , // 7
	VK_START      , // 8
	VK_SELECT     , // 9
	VK_FL         , // 10
	VK_FR         , // 11
	VK_FA         , // 12
	VK_FB         , // 13
	VK_FX         , // 14
	VK_FY         , // 15
	VK_VOL_UP     , // 16
	VK_VOL_DOWN   , // 17
	VK_TAT          // 18
};


class Hamur
{

public:
	static SDL_Surface* load_image( string file_name, int red, int green, int blue );
	static SDL_Surface* load_image( string file_name);
	static void draw_surface(int x, int y, SDL_Surface *source, SDL_Surface *target,
     SDL_Rect *rect);
    static void draw_surface_once(int x, int y, SDL_Surface *source, SDL_Surface *target,
     SDL_Rect *rect);

	static void draw_surface( int x, int y, SDL_Surface *source, SDL_Surface *target);
	static void draw_surface_once( int x, int y, SDL_Surface *source, SDL_Surface *target);
	static bool init( string, int width, int height, int bpp, int fps, Uint32 flag);
	static void error_message_handler( string );
	static void message_handler( string );
	static void hamur_quit();
	static SDL_Surface* getScreen();
	static int getFPSConstant();
	static int getScreen_BPP();
	static int getScreen_WIDTH();
	static int getScreen_HEIGHT();
	static bool check_collision(const SDL_Rect &, const SDL_Rect &);
	static Mix_Music* load_music( string file_name);
	static Mix_Chunk* load_soundfx(string filename);
	static Uint32 getpixel(SDL_Surface *surface, int x, int y);
	static void putpixel(SDL_Surface *surface, int x, int y, Uint32 colorRGB);
	static void lock_surface(SDL_Surface *surface);
	static void unlock_surface(SDL_Surface *surface);
	static void set_alpha(SDL_Surface *surface, int alphaValue);
	static void draw_circle(SDL_Surface *surface, int x, int y, int radius, Uint32 colorRGB);
	static void fill_circle(SDL_Surface *surface, int x, int y, int radius, Uint32 colorRGB);
	static void draw_line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colorRGB);
	static TTF_Font* load_font(string file_name, int size);
	static SDL_Joystick* open_joystick();
	static void rotate(int corX, int corY, int refX, int refY, float degree,
		SDL_Surface *source, SDL_Surface *target);
	static void rotate(int corX, int corY, int refX, int refY, int deg360,
		SDL_Surface *source, SDL_Surface *target);
	static void rotate(int corX, int corY, int refX, int refY, float degree,
		SDL_Surface *source, SDL_Surface *target, Uint32 colorkey);
	static void rotate(int corX, int corY, int refX, int refY, int deg360,
		SDL_Surface *source, SDL_Surface *target, Uint32 colorkey);
	static void x_simetri(SDL_Surface *tsurface);
	static int random_generator(int bottom, int top);
	static void fill_rect(SDL_Surface *surface, int x, int y, int w, int h,
		int r, int g, int b);

	// Gp2x
    static void hamur_quit_gp2x();


private:

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static int SCREEN_BPP;
	static int FRAMES_PER_SECOND;
	static SDL_Surface *SCREEN;

};

#endif // HAMUR_H
