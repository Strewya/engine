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

#define CORE_GAME_BUILD HEDGEHOG

#if CORE_GAME_BUILD == PONG
#define CORE_GAME Pong
#elif CORE_GAME_BUILD == HEDGEHOG
#define CORE_GAME Hedgehog
#endif

#define CORE_GAME_HEADER <Games/CORE_GAME/CORE_GAME.h>