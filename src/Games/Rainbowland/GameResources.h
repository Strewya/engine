#pragma once
/********************************************
*  contents:   GameResources
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Audio/FmodSoundHandle.h>
#include <Graphics/Texture/DXTextureHandle.h>
#include <Graphics/Shader/DXShaderHandle.h>
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
