#pragma once
/********************************************
*	class:	InputBindings
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class RainbowlandGame;

	void setupDebugInputBindings(RainbowlandGame& game);

	void registerKeyboardMouse(RainbowlandGame& game, uint32_t playerID);
	void registerGamepad(RainbowlandGame& game, uint32_t playerID, uint32_t gamepadID);
}
