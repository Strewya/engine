//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "games/rainbowland/load_game_resources.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "audio/audio_system.h"
#include "graphics/graphics_system.h"
#include "utility/utility.h"
#include "games/rainbowland/logic/code.h"
/******* end headers *******/

namespace core
{
   GameResources loadGameResources(AudioSystem* sfx, GraphicsSystem* gfx)
   {
      GameResources assets{};
      assets.atlas = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      assets.background = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/background.png"));
      assets.font = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/font_t.png"));

      assets.mainVS = gfx->loadVertexShaderFromFile(CORE_RESOURCE("Shaders/shader_vs.cso"), VertexType::Health);
      assets.mainPS = gfx->loadPixelShaderFromFile(CORE_RESOURCE("Shaders/shader_ps.cso"));
      assets.healthPS = gfx->loadPixelShaderFromFile(CORE_RESOURCE("Shaders/health_ps.cso"));
      
      assets.reload = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/reload.wav"));
      assets.pistol = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/pistol.wav"));
      assets.shotgun = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/shotgun.wav"));
      assets.uzi = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/uzi.wav"));
      assets.sniper = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/sniper.wav"));
      assets.rpg = audio::loadSoundFromFile(sfx, CORE_RESOURCE("Sounds/missile.wav"));

      return assets;
   }

   bool checkGameResourcesLoaded(GameResources* assets)
   {
      bool atlas = assets->atlas.isNull() == false;
      bool background = assets->background.isNull() == false;
      bool font = assets->font.isNull() == false;
      bool mainVS = assets->mainVS.isNull() == false;
      bool mainPS = assets->mainPS.isNull() == false;
      bool healthPS = assets->healthPS.isNull() == false;
      bool reload = assets->reload.isNull() == false;
      bool pistol = assets->pistol.isNull() == false;
      bool shotgun = assets->shotgun.isNull() == false;
      bool uzi = assets->uzi.isNull() == false;
      bool sniper = assets->sniper.isNull() == false;
      bool rpg = assets->rpg.isNull() == false;
      
      return (atlas && background && font &&
              mainVS && mainPS && healthPS &&
              reload && pistol && shotgun && uzi && sniper && rpg);
   }

   void unloadGameResources(GameResources* assets, AudioSystem* sfx, GraphicsSystem& gfx)
   {
      audio::unload(sfx, assets->rpg);
      audio::unload(sfx, assets->sniper);
      audio::unload(sfx, assets->uzi);
      audio::unload(sfx, assets->shotgun);
      audio::unload(sfx, assets->pistol);
      audio::unload(sfx, assets->reload);

      gfx.unload(assets->healthPS);
      gfx.unload(assets->mainPS);
      gfx.unload(assets->mainVS);
      gfx.unload(assets->font);
      gfx.unload(assets->background);
      gfx.unload(assets->atlas);
   }
}
