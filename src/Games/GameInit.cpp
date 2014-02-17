//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/GameInit.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/GameSelector.h>

#include CORE_GAME_HEADER
/******* end headers *******/

#define TOKENIZE(a,b) a##b
#define GAME_INST(x) TOKENIZE(x, Game)

namespace Core
{
	void initGame(Window& window)
	{
		doTheLoop<GAME_INST(CORE_GAME)>(window);
	}
}