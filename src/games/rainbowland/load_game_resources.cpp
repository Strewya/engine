//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/load_game_resources.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "graphics/graphics_system.h"
#include "util/utility.h"
#include "games/rainbowland/logic/code.h"
/******* end headers *******/

namespace core
{
   GameResources loadGameResources(SoundManager& sounds, PixelShaderManager& pixelShaders, VertexShaderManager& vertexShaders, TextureManager& textures)
   {
      GameResources assets;
      assets.atlas = textures.loadFromFile(CORE_RESOURCE("wild_bunny_atlas.tif"));
      assets.background = textures.loadFromFile(CORE_RESOURCE("bunny_background.png"));
      assets.font = textures.loadFromFile(CORE_RESOURCE("font_t.png"));

      assets.mainVS = vertexShaders.loadFromFile(CORE_RESOURCE("shader_vs.cso"), HealthVertex::getDescription());
      assets.mainPS = pixelShaders.loadFromFile(CORE_RESOURCE("shader_ps.cso"));

      assets.song = sounds.loadFromFile(CORE_RESOURCE("pisma.mp3"));

      return assets;
   }

   bool checkGameResourcesLoaded(GameResources& assets)
   {
      bool atlas = assets.atlas.isNull() == false;
      bool background = assets.background.isNull() == false;
      bool font = assets.font.isNull() == false;
      bool mainVS = assets.mainVS.isNull() == false;
      bool mainPS = assets.mainPS.isNull() == false;
      bool song = assets.song.isNull() == false;
      
      return (atlas && background && font && mainVS && mainPS && song);
   }

   void unloadGameResources(GameResources& assets, SoundManager& sounds, PixelShaderManager& pixelShaders, VertexShaderManager& vertexShaders, TextureManager& textures)
   {
      sounds.release(assets.song);

      pixelShaders.release(assets.mainPS);
      vertexShaders.release(assets.mainVS);

      textures.release(assets.font);
      textures.release(assets.background);
      textures.release(assets.atlas);
   }
}
