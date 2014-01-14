//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/GameInit.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Pong/PongGame.h>
/******* end headers *******/

namespace Core
{
	void initGame(Window& window)
	{
		doTheLoop<PongGame>(window);
	}
}