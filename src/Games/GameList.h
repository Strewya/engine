#pragma once
/********************************************
*  contents:   Game list
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

#define RAINBOWLAND_v2

#if defined PONG

#define CORE_GAME PongGame
#define CORE_PKG $pfile "Games/Pong/Pong.pkg"
#define CORE_HEADER <Games/Pong/Pong.h>


#elif defined HEDGEHOG

#define CORE_GAME HedgehogGame
#define CORE_PKG $pfile "Games/Hedgehog/Hedgehog.pkg"
#define CORE_HEADER <Games/Hedgehog/Hedgehog.h>

#elif defined RAINBOWLAND

#define CORE_GAME RainbowlandGame
#define CORE_PKG $pfile "Games/Rainbowland/Rainbowland.pkg"
#define CORE_HEADER <Games/Rainbowland/Rainbowland.h>

#elif defined RAINBOWLAND_v2

#define CORE_GAME Game
#define CORE_PKG
#define CORE_HEADER <Games/ChaosGarden/Game.h>

#endif

