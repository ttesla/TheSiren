#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "../hamur/hamur.h"
#include "../tufek/tufek.h"
#include "../enemy/air_enemy.h"
#include "../enemy/helicopter.h"
#include "../enemy/bomber.h"

#define MAX_DEMO_PRC 50
#define MAX_MEN_ON_GROUND 10
#define BOSS_LEVEL_FREQ 5

//#define TEST_DEBUG

class Engine
{

public:
	Engine(SDL_Surface *screen);
	~Engine();
	void run();

private:
	void handle_events();
	void detect_collusions();
	void game_menu();
	void game_main();
	void manage_levels(bool reset);
	void manage_objects();
	int manage_bosslevels(bool reset);
	void kill_all_objects();
	int count_ground_parac(); // counts parachuters who are in ground
	void display_score();
	void display_game_info();
	void display_game_stat();
	void display_gameover_msg();
	void check_gameover();
	void master_reset(); // Reset game for new gameplay
	SDL_Event event;

	// Engine variables
	bool quit;
	int frame;
	TTF_Font *font1;
	TTF_Font *font2;
	TTF_Font *font3;
	TTF_Font *font4;
	SDL_Color whiteFontColor;
	SDL_Color overColor;

	// Game objects - variables
	float darken;
	SDL_Surface *screen;
	SDL_Surface *blueBackground;
	SDL_Surface *blackBackground;
	SDL_Surface *menuSurface;
	SDL_Surface *manIcon;
    Tufek tufek;
    vector<AirEnemy *>  airEnemyVector;
    BulletFlak *fBullets;
    int bulletCount;
    int days;
    Mix_Music* sirenMusic;
    Mix_Music* menuMusic;
    int gameState;
    bool mouseFirstButton;
    vector<Parachuter *> demoparachuter;

	// Test & debug variables
	#ifdef TEST_DEBUG
	char fps_str[32];
	char dstr[30];
	char frame_str[32];
	char mouse_msg[32];
	#endif
};

#endif
