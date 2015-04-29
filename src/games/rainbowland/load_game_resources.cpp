//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/load_game_resources.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "games/rainbowland/game_resources.h"
#include "games/rainbowland/game_systems.h"
#include "graphics/graphics_system.h"
#include "util/utility.h"
/******* end headers *******/

namespace Core
{
   GameResources loadGameResources(GameSystems& sys)
   {
      GameResources assets;
      assets.atlas = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      assets.background = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/background.png"));
      assets.font = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/font_t.png"));

      assets.mainVS = sys.gfx->vertexShaders.loadFromFile(CORE_RESOURCE("Shaders/shader_vs.cso"), HealthVertex::getDescription());
      assets.mainPS = sys.gfx->pixelShaders.loadFromFile(CORE_RESOURCE("Shaders/shader_ps.cso"));
      assets.healthPS = sys.gfx->pixelShaders.loadFromFile(CORE_RESOURCE("Shaders/health_ps.cso"));
      
      assets.reload = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/reload.wav"));
      assets.pistol = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/pistol.wav"));
      assets.shotgun = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/shotgun.wav"));
      assets.uzi = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/uzi.wav"));
      assets.sniper = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/sniper.wav"));
      assets.rpg = sys.audio->sounds.loadFromFile(CORE_RESOURCE("Sounds/missile.wav"));

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

   void unloadGameResources(GameSystems& sys, GameResources& assets)
   {
      sys.audio->sounds.release(assets.rpg);
      sys.audio->sounds.release(assets.sniper);
      sys.audio->sounds.release(assets.uzi);
      sys.audio->sounds.release(assets.shotgun);
      sys.audio->sounds.release(assets.pistol);
      sys.audio->sounds.release(assets.reload);

      sys.gfx->pixelShaders.release(assets.healthPS);
      sys.gfx->pixelShaders.release(assets.mainPS);
      sys.gfx->vertexShaders.release(assets.mainVS);
      sys.gfx->textures.release(assets.font);
      sys.gfx->textures.release(assets.background);
      sys.gfx->textures.release(assets.atlas);
   }
}
