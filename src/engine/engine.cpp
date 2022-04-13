#include "engine.h"

Engine::Engine(SDL_Surface *screen)
{
	// Engine variables
	this->screen = screen;
	frame  = 0;
	quit   = false;
	font1  = Hamur::load_font("res/font/font1.ttf",17);
	font2  = Hamur::load_font("res/font/font2.ttf",30);
	font3  = Hamur::load_font("res/font/font2.ttf",80);
	font4  = Hamur::load_font("res/font/font1.ttf",20);

	// Fonts
	whiteFontColor.r = 220; whiteFontColor.g = 220; whiteFontColor.b = 220;
	overColor.r = 200; overColor.g = 0; overColor.b = 0;

	// Game objects - variables
	blueBackground  = Hamur::load_image("res/gfx/back01.jpg");
	menuSurface	    = Hamur::load_image("res/gfx/menu.jpg");
	blackBackground = Hamur::load_image("res/gfx/back02.jpg");
	manIcon 		= Hamur::load_image("res/gfx/manIcon.png", 255, 0, 255);

	for(int i = 0; i < MAX_DEMO_PRC; ++i)
	{
		Parachuter *prc = new Parachuter();
		prc->setParachuter(Hamur::random_generator(0, Hamur::getScreen_WIDTH() - 50),
			-Hamur::random_generator(600, 10000), 3 , 1 , Hamur::random_generator(10, 250));
		demoparachuter.push_back(prc);
	}

	darken      = 0;
	gameState   = 0;
	days 		= 0; // A kind of game score
	mouseFirstButton = false; // Button not pressed.
	fBullets = tufek.get_bullets();
	bulletCount = tufek.getBulletCount();

	sirenMusic = Hamur::load_music("res/sfx/siren.wav");
	menuMusic  = Hamur::load_music("res/sfx/raiden.xm");
}


Engine::~Engine()
{
	SDL_FreeSurface(blueBackground);
	SDL_FreeSurface(blackBackground);
	SDL_FreeSurface(menuSurface);
	SDL_FreeSurface(manIcon);
	Mix_FreeMusic(sirenMusic);
	Mix_FreeMusic(menuMusic);
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);
	TTF_CloseFont(font3);
	TTF_CloseFont(font4);

	// Clean animation objects.
	for(int i = 0; i < MAX_DEMO_PRC; ++i)
	{
		delete demoparachuter[i];
	}
	demoparachuter.clear();

	kill_all_objects(); // Final cleaning
}

// EVENTS
void Engine::handle_events()
{
	while( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT ) quit = true;

		else if(event.type == SDL_KEYDOWN) // ESC key
		{
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				if(gameState == 1)
				{
					master_reset();
					gameState = 0;
					Mix_PlayMusic(menuMusic, -1);
				}
				else if(gameState == 0) quit = true;
			}

			else if(event.key.keysym.sym == SDLK_RETURN)
			{
				if(gameState == 0)
				{
					gameState = 1;
					Mix_FadeOutMusic(5000);
				}
			}
		}

		else if( event.type == SDL_MOUSEMOTION )
		{
			int mX = event.motion.x;
            int mY = event.motion.y;

			tufek.tufekMove(mX, mY);

			#ifdef TEST_DEBUG
			float dg = tufek.getDegree();
			sprintf(dstr, "Derece: %d:", (int)(dg * 180 / PI));
			sprintf(mouse_msg, "X: %d Y: %d", mX, mY);
			#endif
		}

		else if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT) // left mouse button
                mouseFirstButton = true;
        }

        else if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == SDL_BUTTON_LEFT) // left mouse button
                mouseFirstButton = false;
        }
	}

	if(mouseFirstButton) // mouseFirstButton = true?
		tufek.fire();
}

void Engine::detect_collusions()
{
	for(int i = 0; i < bulletCount; ++i)
	{
		for(int j = 0; j < airEnemyVector.size(); ++j)
		{
			SDL_Rect rect1, rect2;
			rect1 = fBullets[i].getRect();
			rect2 = airEnemyVector[j]->getRect();

			// All flying crafts versus flak bullets
			if(Hamur::check_collision(rect1, rect2))
			{
				fBullets[i].hit();
				airEnemyVector[j]->hit();
			}

			// Helicopter's parachuters versus flak bullets
			// getParachuterCount 0 oldugu icin gizli bir NULL kontrolu var... :)
			// Boyle olmasaydi bu Helicopter mi? yada NULL mu diye bakmak lazimdi
			for(int k = 0; k < airEnemyVector[j]->getParachuterCount(); ++k)
			{
				rect2 = airEnemyVector[j]->getParachuters()[k].get_man_rect();

				if(Hamur::check_collision(rect1, rect2))
				{
					fBullets[i].hit();
					airEnemyVector[j]->getParachuters()[k].man_hit();
				}

				rect2 = airEnemyVector[j]->getParachuters()[k].get_parachute_rect();

				if(Hamur::check_collision(rect1, rect2))
				{
					fBullets[i].hit();
					airEnemyVector[j]->getParachuters()[k].parac_hit();
				}
			}


			// Bomber's big bomb versus flak bullets
			if(airEnemyVector[j]->getBomberBullet() != NULL)
			{
				rect2 = airEnemyVector[j]->getBomberBullet()->getRect();

				if(Hamur::check_collision(rect1, rect2))
				{
					fBullets[i].hit();
					airEnemyVector[j]->getBomberBullet()->hit();
				}
			}

			if(airEnemyVector[j]->getBomberBullet() != NULL)
			{
				rect1 = tufek.get_rect();
				rect2 = airEnemyVector[j]->getBomberBullet()->getRect();

				if(Hamur::check_collision(rect1, rect2))
				{
					tufek.hit();
					airEnemyVector[j]->getBomberBullet()->hit();
				}
			}
		} // Air crafts for loop
	} // Bullets for loop
}


int Engine::count_ground_parac()
{
	int groundCount = 0;

	for(int i = 0; i < airEnemyVector.size(); ++i)
	{
		for(int j = 0; j < airEnemyVector[i]->getParachuterCount(); ++j)
		{
			if( airEnemyVector[i]->getParachuters()[j].is_ground() )
				groundCount++;
		}
	}

	return groundCount;
}


void Engine::kill_all_objects()
{
	for(int i = 0; i < airEnemyVector.size(); ++i)
	{
		if(airEnemyVector[i] != 0)
		{
			//printf("DELETED OBJECT: %d\n", i);
			delete airEnemyVector[i];
		}
	}

	airEnemyVector.clear();
}


void Engine::manage_objects()
{
	for(int i=0; i < airEnemyVector.size(); ++i)
	{
		airEnemyVector[i]->move();
		airEnemyVector[i]->draw(screen);
	}
}


int Engine::manage_bosslevels(bool reset)
{
	static bool playSiren = true;
	static int jet = 1;
	static int strike = 0;
	static int letTimePass = 0;

	if(reset)
	{
		playSiren = true;
		jet = 1;
		strike = 0;
		letTimePass = 0;
		return 0; // dont forget, it may be fatal!
	}

	if(playSiren)
	{
		Mix_PlayMusic(sirenMusic, 1);
		playSiren = false;
	}

	if(strike < jet && letTimePass > 1500)
	{
		int direction = Hamur::random_generator(0, 2);
		int xCor = Hamur::random_generator(200, 500);
		int yCor = Hamur::random_generator(5, 100);
		int velocity = Hamur::random_generator(2, 5);
		int dropX = Hamur::random_generator(100, Hamur::getScreen_WIDTH() - 100);

		if(direction == 0)
			airEnemyVector.push_back(new Bomber(-120 - xCor, yCor, 396, 527, velocity, dropX));
		else
			airEnemyVector.push_back(new Bomber(920 + xCor, yCor, 396, 527, -velocity, dropX));

		strike++;
	}

	// Let the sky darken
	darken += 0.2f;
	if(darken > 255.0f) darken = 255.0f;

	letTimePass++;

	if(letTimePass > 3000)
	{
		playSiren = true;
		jet++;
		strike = 0;
		letTimePass = 0;
		return 0;
	}

	return BOSS_LEVEL_FREQ;
}

void Engine::manage_levels(bool reset)
{
	static int sekans = 200;
	static int counter = 0;
	static int minParac = 0;
	static int maxParac = 4;
	static int interVal = 600;
	static int airStrike = 0;

	// Reset game...
	if(reset)
	{
		sekans = 200;
		counter = 0;
		minParac = 0;
		maxParac = 4;
		interVal = 600;
		airStrike = 0;
		return; // dont forget, it may be fatal!
	}

	if(airStrike == BOSS_LEVEL_FREQ && !tufek.is_hit())
	{
		airStrike = manage_bosslevels(false);

		// Increase difficulty
		if(airStrike == 0)
		{
			minParac = 1;
			maxParac++;
			days++; // increase days - score
		}
	}

	else if(counter == sekans && !tufek.is_hit())
	{
		int negX = -150;
		int posX = 850;
		int posY1 = 5;
		int posY2 = 60;
		int vel1 = Hamur::random_generator(1, 3);
		int vel2 = Hamur::random_generator(1, 3);
		int parac = Hamur::random_generator(minParac, maxParac);

		airEnemyVector.push_back(new Helicopter(negX, posY1, vel1, parac));
		airEnemyVector.push_back(new Helicopter(posX, posY2, -vel2, parac));

		//Increase difficulty
		interVal -= 5;
		if(interVal <= 400) interVal = 400;

		sekans += interVal;
		airStrike++;

		//printf("INTERVAL: %d\n", interVal);
	}

	else
	{
		// Let the sky brighten
		darken -= 0.2f;

		if(darken < 0) darken = 0;

		counter++;
	}
}

void Engine::display_gameover_msg()
{
	Hamur::draw_surface_once(180, 250,
		TTF_RenderText_Solid(font3,"Game Over", overColor ), screen);

	//Hamur::draw_surface(180,250,overSurface,screen);
}

void Engine::check_gameover()
{
	if(tufek.is_hit())
	{
		display_gameover_msg();
	}

	else if(count_ground_parac() >= MAX_MEN_ON_GROUND)
		tufek.hit();
}


void Engine::game_menu()
{
	Hamur::draw_surface(0, 0, menuSurface, screen);

	// Some nifty animation
	for(int i = 0; i < MAX_DEMO_PRC; ++i)
	{
		demoparachuter[i]->move();
		demoparachuter[i]->draw(screen);
	}

	display_game_info();
}

// Display information about the game. (Some basic help and credits)
void Engine::display_game_info()
{
	static char *gameInfo1 = "War has struck your country and enemy forces are invading your lands";
	static char *gameInfo2 = "- Do not let more and 10 paratroopers land on your base.";
	static char *gameInfo3 = "- Keep an eye on your flak temper. If it heats, you can't shoot.";
	static char *gameInfo4 = "- The only thing you need is courage and your MOUSE first button!";
	static char *startMsg = "PRESS ENTER TO START!";
	static char *scrollStr = "CODE: Daghan Demirci, Omer Akyol and some additional code Goksel Goktas -- GFX: Tayfun Tuna -- SFX: Intro music from legendary game RAIDEN ----- This game has evolved so much. First it was developed as a small project for the PC only, then at Nightshift'07 demoparty it has ported to GP2X. Yeah! there is still more history. After then we as INSPIRE13 (Omer 'tesla' Akyol & Daghan 'chaosmaker' Demirci) thought that we should make a final release of this game. And we cleared many gameplay bugs, added some more eye candy and game rules. So this is the long story of the mighty Siren. Hope you enjoy our game. INSPIRE13 team";
	static int harmonicAlpha = 10;
	static int harmonicWidth = 0;
	static int scrollOffset = 0;

	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() - 670, 150,
	    TTF_RenderText_Solid(font1, gameInfo1, whiteFontColor ), screen);

	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() - 670, 170,
	    TTF_RenderText_Solid(font1, gameInfo2, whiteFontColor ), screen);

	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() - 670, 190,
	    TTF_RenderText_Solid(font1, gameInfo3, whiteFontColor ), screen);

	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() - 670, 210,
	    TTF_RenderText_Solid(font1, gameInfo4, whiteFontColor ), screen);

	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() - scrollOffset, 550,
	    TTF_RenderText_Solid(font1, scrollStr, whiteFontColor), screen);

	scrollOffset++;

	if(harmonicWidth > 250 || harmonicWidth < 0)
		harmonicAlpha = -harmonicAlpha;

		harmonicWidth += harmonicAlpha;

	SDL_Surface *fSurface = TTF_RenderText_Solid(font2, startMsg, overColor );
	Hamur::set_alpha(fSurface, harmonicWidth);
	Hamur::draw_surface_once(Hamur::getScreen_WIDTH() / 2 - 150, 330, fSurface, screen);
}

void Engine::display_score()
{
	static char tempStr[50];

	sprintf(tempStr, "DAYS RESISTED: %d", days);

	Hamur::draw_surface_once(5, 570,
	    TTF_RenderText_Solid(font2, tempStr, whiteFontColor ), screen);
}


void Engine::display_game_stat()
{
	static char tempStr1[50];
	static SDL_Color tempColor;

	int paracCount = count_ground_parac();

	if(paracCount >= MAX_MEN_ON_GROUND - 2) // 8 is the warning level.
	{
		tempColor.r += 5;

		if(tempColor.r >= 240) tempColor.r = 85;

		tempColor.g = 0; tempColor.b = 0;
	}

	else
	{
		tempColor.r = 240; tempColor.g = 240; tempColor.b = 240;
	}

	sprintf(tempStr1, "x%02d)", paracCount);

	Hamur::draw_surface_once(2, 2,
	   	TTF_RenderText_Solid(font4, "(", tempColor), screen);

	Hamur::draw_surface(7, -12, manIcon, screen);

	Hamur::draw_surface_once(32, 2,
	   	TTF_RenderText_Solid(font4, tempStr1, tempColor), screen);
}


// The handy dandy reset game function
void Engine::master_reset()
{
	darken      = 0;
	days 		= 0; // Game score
	mouseFirstButton = false; // Button not pressed.
	manage_levels(true);      // Reset level manager
	manage_bosslevels(true);  // Reset boss level manager
	tufek.reset();
	kill_all_objects();
	Mix_HaltChannel(-1); // Stop all effects playing.
}


void Engine::game_main()
{
	Hamur::draw_surface(0, 0, blueBackground, screen);
	Hamur::set_alpha(blackBackground, (int)darken);
	Hamur::draw_surface(0, 0, blackBackground, screen);

	manage_levels(false);  // manage game levels
	manage_objects();      // manage air objects
	tufek.draw(screen);    // draw tufek
	detect_collusions();   // check collusions
	display_score();	   // show score
	display_game_stat();   // show game stats on the top corner.
	check_gameover();      // Check whether game is over or not?!

	#ifdef TEST_DEBUG
	Hamur::draw_surface_once(5, 70,
	    TTF_RenderText_Solid(font1, dstr, whiteFontColor ), screen);
	Hamur::draw_surface_once(5, 0,
	    TTF_RenderText_Solid(font1, fps_str, whiteFontColor ), screen);
	Hamur::draw_surface_once(5, 20,
	    TTF_RenderText_Solid(font1, frame_str, whiteFontColor ), screen);
	Hamur::draw_surface_once(5, 40,
	   	TTF_RenderText_Solid(font1, mouse_msg, whiteFontColor ), screen);
	#endif
}


void Engine::run()
{
	Mix_PlayMusic(menuMusic, -1);

	// super while...
	while(quit == false)
	{
		handle_events();

		// Clear - Refresh screen //
		static Uint32 blackRGB = SDL_MapRGB( screen->format, 0, 0, 0 );
		SDL_FillRect( screen, &screen->clip_rect, blackRGB);

		// FPS //
		static int fps = 0, stime = 0, temp = 0, fpsFrame = 0; // fps variables
		temp = SDL_GetTicks() - stime;
		if(temp != 0) fps = 1000 * fpsFrame / temp;
		if(fps > Hamur::getFPSConstant()) continue; // fps regulator
		if(fpsFrame > 20) // 20 is the Accuracy of FPS regulator
		{
			fpsFrame = 0;
			stime = SDL_GetTicks();
		}
		#ifdef TEST_DEBUG
		sprintf(fps_str, "FPS: %d", fps);
		sprintf(frame_str, "Frame: %d", frame);
		#endif
		// FPS //

		// GAME STATES
		if(gameState == 0) // MENU
			game_menu();
		else if(gameState == 1) // MAIN GAME
			game_main();

		SDL_Flip(screen); // Update the screen

		// Save screen as BMP for video output, ~30fps
		/*if(frame % 3 == 0)
		{
			char fileStr[50];
			sprintf(fileStr, "output%d.bmp", frame);
			SDL_SaveBMP(screen, fileStr);
		}*/

		frame++;    // for general usage
		fpsFrame++; // for FPS calculation
	} //while
}
