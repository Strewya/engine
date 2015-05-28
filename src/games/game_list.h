#pragma once
/********************************************
*  contents:   list of games that could be run
*  usage:      note that most of these do not work right now...
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

#define RAINBOWLAND_v2

#if defined PONG
//does not work
#define CORE_GAME PongGame
#define CORE_PKG $pfile "games/pong/pong.pkg"
#define CORE_HEADER "games/pong/pong.h"


#elif defined HEDGEHOG
//does not work
#define CORE_GAME HedgehogGame
#define CORE_PKG $pfile "games/hedgehog/hedgehog.pkg"
#define CORE_HEADER "games/hedgehog/hedgehog.h"

#elif defined RAINBOWLAND
//does not work
//is being replaced
#define CORE_GAME RainbowlandGame
#define CORE_PKG $pfile "games/rainbowland/rainbowland.pkg"
#define CORE_HEADER "games/rainbowland/rainbowland.h"

#elif defined RAINBOWLAND_v2

#define CORE_GAME Game
#define CORE_PKG
#define CORE_HEADER "games/rainbowland/game.h"

#else

#define NO_GAME
#define CORE_PKG
#define CORE_HEADER "games/game_list.h"

#endif

