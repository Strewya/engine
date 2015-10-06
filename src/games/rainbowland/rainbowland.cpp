//headers should be ordered alphabetically!
/******* precompiled header *******/
//#include "stdafx.h"
/******* personal header *******/
/******* c++ headers *******/
/******* extra headers *******/
#include "games/rainbowland/rainbowland.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   core_internal GameAssets loadGameAssets(AudioSystem* audio, GraphicsSystem* gfx)
   {
      GameAssets assets{};
      assets.atlas = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      assets.background = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/background.png"));
      assets.font = gfx->loadTextureFromFile(CORE_RESOURCE("Textures/font_t.png"));

      assets.mainVS = gfx->loadVertexShaderFromFile(CORE_RESOURCE("Shaders/shader_vs.cso"), VertexType::Health);
      assets.mainPS = gfx->loadPixelShaderFromFile(CORE_RESOURCE("Shaders/shader_ps.cso"));
      assets.healthPS = gfx->loadPixelShaderFromFile(CORE_RESOURCE("Shaders/health_ps.cso"));

      assets.reload = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/reload.wav"));
      assets.pistol = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/pistol.wav"));
      assets.shotgun = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/shotgun.wav"));
      assets.uzi = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/uzi.wav"));
      assets.sniper = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/sniper.wav"));
      assets.rpg = audio->loadSoundFromFile(CORE_RESOURCE("Sounds/missile.wav"));

      return assets;
   }

   core_internal bool checkGameAssetsLoaded(GameAssets& assets)
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

   core_internal void unloadGameAssets(GameAssets& assets, AudioSystem* audio, GraphicsSystem* gfx)
   {
      gfx->unload(assets.atlas);
      gfx->unload(assets.background);
      gfx->unload(assets.font);
      gfx->unload(assets.mainVS);
      gfx->unload(assets.mainPS);
      gfx->unload(assets.healthPS);

      audio->unload(assets.reload);
      audio->unload(assets.pistol);
      audio->unload(assets.shotgun);
      audio->unload(assets.uzi);
      audio->unload(assets.sniper);
      audio->unload(assets.rpg);
   }

   core_internal Game* init_game(Memory mem, CommunicationBuffer* fromMain, CommunicationBuffer* toMain, AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script)
   {
      Game* game = emplace<Game>(mem);
      game->gameMemory = mem;

      game->assets = loadGameAssets(audio, gfx);
      checkGameAssetsLoaded(game->assets);

      return game;
   }

   core_internal bool tickLogic(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer, AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script, Game* game)
   {
      return false;
   }

   core_internal void tickRender(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer, AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script, Game* game )
   {

   }

   core_internal void shutdown_game(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script, Game* game)
   {
      unloadGameAssets(game->assets, audio, gfx);
   }
}
