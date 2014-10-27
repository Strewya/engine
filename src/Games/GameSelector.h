#pragma once
/********************************************
*	class:	GameSelector
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

#define PONG 0
#define HEDGEHOG 1
#define RAINBOWLAND 2

#define CORE_GAME_BUILD RAINBOWLAND

#if CORE_GAME_BUILD == PONG
#define CORE_GAME Pong
#elif CORE_GAME_BUILD == HEDGEHOG
#define CORE_GAME Hedgehog
#elif CORE_GAME_BUILD == RAINBOWLAND
#define CORE_GAME Rainbowland
#endif

#define CORE_GAME_HEADER <Games/CORE_GAME/CORE_GAME.h>
