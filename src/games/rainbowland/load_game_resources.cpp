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
      assets.atlas = textures.loadFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      assets.background = textures.loadFromFile(CORE_RESOURCE("Textures/background.png"));
      assets.font = textures.loadFromFile(CORE_RESOURCE("Textures/font_t.png"));

      assets.mainVS = vertexShaders.loadFromFile(CORE_RESOURCE("Shaders/shader_vs.cso"), HealthVertex::getDescription());
      assets.mainPS = pixelShaders.loadFromFile(CORE_RESOURCE("Shaders/shader_ps.cso"));
      assets.healthPS = pixelShaders.loadFromFile(CORE_RESOURCE("Shaders/health_ps.cso"));
      
      assets.reload = sounds.loadFromFile(CORE_RESOURCE("Sounds/reload.wav"));
      assets.pistol = sounds.loadFromFile(CORE_RESOURCE("Sounds/pistol.wav"));
      assets.shotgun = sounds.loadFromFile(CORE_RESOURCE("Sounds/shotgun.wav"));
      assets.uzi = sounds.loadFromFile(CORE_RESOURCE("Sounds/uzi.wav"));
      assets.sniper = sounds.loadFromFile(CORE_RESOURCE("Sounds/sniper.wav"));
      assets.rpg = sounds.loadFromFile(CORE_RESOURCE("Sounds/missile.wav"));

      return assets;
   }

   bool checkGameResourcesLoaded(GameResources& assets)
   {
      bool atlas = assets.atlas.isNull() == false;
      bool background = assets.background.isNull() == false;
      bool font = assets.font.isNull() == false;
      bool mainVS = assets.mainVS.isNull() == false;
      bool mainPS = assets.mainPS.isNull() == false;
      bool healthPS = assets.healthPS.isNull() == false;
      bool reload = assets.reload.isNull() == false;
      bool pistol = assets.pistol.isNull() == false;
      bool shotgun = assets.shotgun.isNull() == false;
      bool uzi = assets.uzi.isNull() == false;
      bool sniper = assets.sniper.isNull() == false;
      bool rpg = assets.rpg.isNull() == false;
      
      return (atlas && background && font &&
              mainVS && mainPS && healthPS &&
              reload && pistol && shotgun && uzi && sniper && rpg);
   }

   void unloadGameResources(GameResources& assets, SoundManager& sounds, PixelShaderManager& pixelShaders, VertexShaderManager& vertexShaders, TextureManager& textures)
   {
      sounds.release(assets.rpg);
      sounds.release(assets.sniper);
      sounds.release(assets.uzi);
      sounds.release(assets.shotgun);
      sounds.release(assets.pistol);
      sounds.release(assets.reload);

      pixelShaders.release(assets.healthPS);
      pixelShaders.release(assets.mainPS);
      vertexShaders.release(assets.mainVS);
      textures.release(assets.font);
      textures.release(assets.background);
      textures.release(assets.atlas);
   }
}
