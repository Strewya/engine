//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/LoadGameResources.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/GameResources.h>
#include <Games/Rainbowland/GameSystems.h>
#include <Graphics/GraphicsSystem.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   GameResources loadGameResources(GameSystems& sys)
   {
      GameResources assets;
      assets.atlas = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      assets.background = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/background.png"));
      assets.font = sys.gfx->textures.loadFromFile(CORE_RESOURCE("Textures/font_t.png"));
      assets.mainVS = sys.gfx->shaders.loadVertexShaderFromFile(CORE_RESOURCE("Shaders/shader_vs.cso"), HealthVertex::getDescription());
      assets.mainPS = sys.gfx->shaders.loadPixelShaderFromFile(CORE_RESOURCE("Shaders/shader_ps.cso"));
      assets.healthPS = sys.gfx->shaders.loadPixelShaderFromFile(CORE_RESOURCE("Shaders/health_ps.cso"));

      return assets;
   }

   bool checkGameResourcesLoaded(GameResources& assets)
   {
      bool atlas = assets.atlas.isNull() == false;
      bool background = assets.background.isNull() == false;
      bool font = assets.font.isNull() == false;
      bool mainvs = assets.mainVS.isNull() == false;
      bool mainps = assets.mainPS.isNull() == false;
      bool healthps = assets.healthPS.isNull() == false;
      
      return (atlas && background && mainvs && mainps && healthps && font);
   }

   void unloadGameResources(GameSystems& sys, GameResources& assets)
   {
      sys.gfx->shaders.release(assets.healthPS);
      sys.gfx->shaders.release(assets.mainPS);
      sys.gfx->shaders.release(assets.mainVS);
      sys.gfx->textures.release(assets.font);
      sys.gfx->textures.release(assets.background);
      sys.gfx->textures.release(assets.atlas);
   }
}
