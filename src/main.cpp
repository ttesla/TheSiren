/*
  Authors		: Omer "tesla" Akyol ,
  				  Daghan "chaosmaker" Demirci
  Date			: 25.01.07
  Description	: Paratrooper re-mastered!

  Small fixup	: 18.10.08 - for demonstration.
*/

// INCLUDES
#include "hamur/hamur.h"
#include "engine/engine.h"


// Some SDL initializations
int init_SDL(SDL_Surface **screen)
{
	Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;

    if( Hamur::init("Paratrooper", 800, 600, 32, 60, flags) == false )
	{
		Hamur::error_message_handler("Error in initialize\n");
		return 1;
	}

	*screen = Hamur::getScreen(); // init den sonra cagrilmasi sart

    // SDL_EnableKeyRepeat(1, 10);
	SDL_ShowCursor(SDL_DISABLE); // hide cursor - mouse

	return 0;
}


int main( int argc, char* args[] )
{
	SDL_Surface *screen = NULL; // Main screen

	if( init_SDL(&screen) ) return 1;

	Engine gameEngine(screen); // Init GAME engine
    gameEngine.run(); 		   // Run the GAME

	// Quit
	Hamur::hamur_quit();

	return 0;
}
