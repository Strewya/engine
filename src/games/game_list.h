#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

#define RAINBOWLAND_v2

#if defined PONG

#define CORE_GAME PongGame
#define CORE_PKG $pfile "games/pong/pong.pkg"
#define CORE_HEADER "games/pong/pong.h"


#elif defined HEDGEHOG

#define CORE_GAME HedgehogGame
#define CORE_PKG $pfile "games/hedgehog/hedgehog.pkg"
#define CORE_HEADER "games/hedgehog/hedgehog.h"

#elif defined RAINBOWLAND

#define CORE_GAME RainbowlandGame
#define CORE_PKG $pfile "games/rainbowland/rainbowland.pkg"
#define CORE_HEADER "games/rainbowland/rainbowland.h"

#elif defined RAINBOWLAND_v2

#define CORE_GAME Game
#define CORE_PKG
#define CORE_HEADER "games/rainbowland/game_main.h"

#else

#define NO_GAME
#define CORE_PKG
#define CORE_HEADER "games/game_list.h"

#endif

