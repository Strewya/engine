#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <audio/fmod_sound_handle.h>
#include <graphics/texture/texture_handle.h>
#include <graphics/shader/shader_handle.h>
/******* end header inclusion *******/

namespace Core
{
   class GameResources
   {
   public:
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
}
