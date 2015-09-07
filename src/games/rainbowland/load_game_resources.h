#pragma once
/********************************************
*  contents:   functions for loading, validity check and unloading of game resources
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/audio_system.h"
#include "graphics/shader/pixel/pixel_shader_manager.h"
#include "graphics/shader/vertex/vertex_shader_manager.h"
#include "graphics/texture/texture_manager.h"
/******* end header inclusion *******/

namespace core
{
   struct GameResources;

   GameResources loadGameResources(AudioSystem& sounds, PixelShaderManager& pixelShaders, VertexShaderManager& vertexShaders, TextureManager& textures);
   bool checkGameResourcesLoaded(GameResources& assets);
   void unloadGameResources(GameResources& assets, AudioSystem& sounds, PixelShaderManager& pixelShaders, VertexShaderManager& vertexShaders, TextureManager& textures);
}
