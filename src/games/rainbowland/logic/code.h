#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
#include "graphics/texture/texture_handle.h"
#include "graphics/shader/shader_handle.h"
#include "audio/fmod_sound_handle.h"
#include "graphics/font/font_descriptor.h"
#include "graphics/camera.h"
/******* end header inclusion *******/

namespace core
{
   struct GameResources
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

   struct Time
   {
      uint32_t deltaMicrosReal;
      uint32_t deltaMicrosVirt;
      float deltaTimeReal;
      float deltaTimeVirt;
   };

   struct GameState
   {
      enum class GlobalGameState
      {
         MainMenu,
         Gameplay,
         Score
      };

      enum class GameplayState
      {
         //ColorPick,
         ClassPick,
         Session
      };

      GlobalGameState globalGameState;
      GameplayState gameplayState;
      Camera camera;
      // #test
      FontDescriptor fontDesc;
   };
}
