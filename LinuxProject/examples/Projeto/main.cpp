#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>

#include "Game.h"
#include "SplashState.h"

int main( int argc, const char* argv[] )
{

    cgf::Game game(5, 30);
    game.init("Top Down Shooter", 1600, 900, false);

    game.changeState(SplashState::instance());

	while(game.isRunning())
	{
		game.handleEvents();
		game.update();
		game.draw();
	}

	// cleanup the engine
	game.clean();

    return EXIT_SUCCESS;
}
