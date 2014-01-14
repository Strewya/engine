#pragma once
/********************************************
*	class:	
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Window/Window.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;

	void initGame(Window& window);

	template<typename GAME> void doTheLoop(Window& window)
	{
		GAME game;

		bool running = game.init(window);
		while(running)
		{
			running = game.tick();
		}
		game.shutdown();
		window.close();
	}
}