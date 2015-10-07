#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/graphics_system.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   /************************************************************************
    *              ENUMS
    ************************************************************************/

   enum class State
   {
      MainMenu,
      GameplaySetup,
      GameplaySession,
      Score,
      Quit,
      Startup,
      Shutdown
   };

   struct GameAssets
   {
      HTexture atlas;
      HTexture background;
      HTexture font;
      HVertexShader mainVS;
      HPixelShader mainPS;
      HPixelShader healthPS;
      HSound reload;
      HSound pistol;
      HSound uzi;
      HSound rpg;
      HSound sniper;
      HSound shotgun;
   };

   struct World
   {

   };

   struct CursorState
   {
      bool show;
      bool lock;
      bool relativeMovement;
   };

   struct Game
   {
      Memory gameMemory;
      GameAssets assets;

      State currentState;
      State nextState;

      bool isPaused;
      
      CursorState cursorState;
   };

   struct Time
   {
      u32 micros;
      f32 seconds;
   };

   struct DeltaTime
   {
      Time real;
      Time virt;
   };
}
