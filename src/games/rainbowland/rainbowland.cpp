//headers should be ordered alphabetically!
/******* precompiled header *******/
//#include "stdafx.h"
/******* personal header *******/
/******* c++ headers *******/
/******* extra headers *******/
#include "input/mouse.h"
#include "games/rainbowland/rainbowland.h"
#include "utility/collision_checks.h"
#include "utility/color.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   enum
   {
      Cursor_Lock = true,
      Cursor_Unlock = false,
      Cursor_Hide = false,
      Cursor_Show = true,
      Cursor_Absolute = false,
      Cursor_Relative = true,
   };

   core_internal void updateCursorInfo(CommunicationBuffer* toMain, bool show, bool lock, bool relative)
   {
      WinMsg msg;
      msg.type = WinMsgType::ShowCursor;
      msg.showCursor = show;
      toMain->writeEvent(msg);
      msg.type = WinMsgType::LockCursor;
      msg.showCursor = lock;
      toMain->writeEvent(msg);
      msg.type = WinMsgType::RelativeCursor;
      msg.showCursor = relative;
      toMain->writeEvent(msg);
   }

   core_internal void onLostFocus(CommunicationBuffer* toMain, Game* game)
   {
      game->isPaused = true;
      updateCursorInfo(toMain, Cursor_Show, Cursor_Unlock, Cursor_Absolute);
   }

   core_internal void onGainFocus(CommunicationBuffer* toMain, Game* game)
   {
      game->isPaused = false;
      updateCursorInfo(toMain, game->sharedData.cursorState.show, game->sharedData.cursorState.lock, game->sharedData.cursorState.relativeMovement);
   }

   core_internal void onFileChange(WinMsg& msg,
                                   AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                   Game* game)
   {
      if( msg.fileChange.action == FileChangeType::Modified )
      {
         //normalize string first, turning \ to /, 
         char* str = msg.fileChange.name;
         const char* ext = nullptr;
         while( *str )
         {
            if( *str == '\\' )
            {
               *str = '/';
            }
            if( *str == '.' )
            {
               ext = str + 1;
            }
         }

         if( strcmp(ext, "png") == 0 || strcmp(ext, "bmp") == 0 || strcmp(ext, "tif") == 0 || strcmp(ext, "jpg") == 0 )
         {
            //if( graphicsSystem.textures.isLoaded(msg.fileChange.name) )
            {
               //graphicsSystem.textures.reloadFromFile(msg.fileChange.name);
            }
         }
         else if( strcmp(ext, "cso") == 0 )
         {
            //if( graphicsSystem.pixelShaders.isLoaded(msg.fileChange.name) )
            {
               //graphicsSystem.pixelShaders.reloadFromFile(msg.fileChange.name);
            }
            //else if( graphicsSystem.vertexShaders.isLoaded(msg.fileChange.name) )
                     {
                        //graphicsSystem.vertexShaders.reloadFromFile(msg.fileChange.name, HealthVertex::getDescription());
                     }
         }
         else if( strcmp(ext, "wav") == 0 || strcmp(ext, "mp3") == 0 )
         {
            //if( audioSystem.sounds.isLoaded(file.c_str()) )
            //{
            //   audioSystem.sounds.reloadFromFile(file.c_str());
            //}
         }
      }
   }

   /************************************************************************
    *              GUI RELATED
    ************************************************************************/

   inline v2 orthoMousePosition(MousePosition mouse, f32 windowWidth, f32 windowHeight)
   {
      v2 result;

      result.x = mouse.x - windowWidth / 2;
      result.y = -mouse.y + windowHeight / 2;

      return result;
   }

   core_internal void handle_mouseMove(GuiData& gui, v2 mousePosition)
   {
      u32 buttonCount = gui.button.count;
      gui.hoverButton = buttonCount;
      for( auto i = 0U; i < buttonCount; ++i )
      {
         auto result = checkCollision(mousePosition, Rect{gui.button.position[i], gui.button.halfsize[i]});
         if( result.isColliding )
         {
            gui.hoverButton = i;
         }
      }
   }

   core_internal void handle_mouseDown(GuiData& gui)
   {
      gui.hotButton = gui.hoverButton;
   }

   core_internal u32 handle_mouseUp(GuiData& gui)
   {
      u32 activateButton = gui.button.count;
      if( gui.hotButton == gui.hoverButton )
      {
         activateButton = gui.hotButton;
      }
      gui.hotButton = gui.button.count;
      return activateButton;
   }

   core_internal u32 handle_guiInput(GuiData& gui, SharedData& shared, const Constants& constants, std::array<WinMsg, 10>& frameEvents)
   {
      u32 activatedButton = gui.button.count;
      for( auto event : frameEvents )
      {
         switch( event.type )
         {
            case WinMsgType::MouseMove:
            {
               if( !event.mouse.move.relative )
               {
                  shared.mousePosition = orthoMousePosition(event.mouse.position, constants.windowWidth, constants.windowHeight);
               }
               else
               {
                  shared.mousePosition += {(f32)event.mouse.position.x, -(f32)event.mouse.position.y};
               }
               handle_mouseMove(gui, shared.mousePosition);
            } break;

            case WinMsgType::MouseButton:
            {
               if( event.mouse.button.id == Mouse::LeftButton )
               {
                  if( event.mouse.button.isDown )
                  {
                     handle_mouseDown(gui);
                  }
                  else
                  {
                     activatedButton = handle_mouseUp(gui);
                  }
               }
            } break;
         }
      }
      return activatedButton;
   }

   core_internal void render_guiData(GuiData& gui, SharedData& shared, const GameAssets& assets, GraphicsSystem& gfx)
   {
      gfx.setOrthographicProjection();

/*
      for( auto i = 0U; i < BUTTONS; ++i )
      {
         Transform buttonTransform{gui.button.position[i]};
         Color color = gui.button.idleColor[i];
         if( i == gui.hoverButton )
         {
            color = gui.button.hoverColor[i];
            if( i == gui.hotButton )
            {
               color = gui.button.hotColor[i];
            }
         }
         auto buttonFrameMesh = makeOutlineQuad({}, gui.button.halfsize[i], assets.mainVS, assets.mainPS);
         gfx.renderMesh(buttonTransform, color, buttonFrameMesh);

         auto buttonTextMesh = font.makeTextMesh(gui.button.caption[i].c_str(), shared.font, {1, 1}, TextJustification::Center, TextJustification::Middle);
         gfx.renderMesh(buttonTransform, {}, buttonTextMesh);
      }

      auto cursorMesh = makeSolidCircle({}, 3, 16, assets.mainVS, assets.mainPS);
      gfx.renderMesh({shared.mousePosition}, {}, cursorMesh);
*/
   }

   /************************************************************************
    *              MAIN MENU RELATED
    ************************************************************************/
   core_internal void initState_mainMenu(MainMenuData& state, SharedData& shared, const Constants& constants, GameAssets& assets)
   {
      state.buttonFunctionToExecute = MainMenuData::COUNT;

      state.gui.hoverButton = state.COUNT;
      state.gui.hotButton = state.COUNT;

      Color defaultIdleColor{1, 1, 1};
      Color defaultHoverColor{1, 1, 0};
      Color defaultHotColor{1, 0, 0};



      auto i = 0;
      state.gui.button.position[i++] = {0, 100};
      state.gui.button.position[i++] = {0, -100};

      i = 0;
      state.gui.button.halfsize[i++] = {200, 50};
      state.gui.button.halfsize[i++] = {200, 50};

      i = 0;
      state.gui.button.idleColor[i++] = {0, 1, 0};
      state.gui.button.idleColor[i++] = {0, 1, 1};

      i = 0;
      state.gui.button.hoverColor[i++] = defaultHoverColor;
      state.gui.button.hoverColor[i++] = defaultHoverColor;

      i = 0;
      state.gui.button.hotColor[i++] = defaultHotColor;
      state.gui.button.hotColor[i++] = defaultHotColor;

      i = 0;
      state.gui.button.caption[i++] = "Start game";
      state.gui.button.caption[i++] = "Quit";

   }

   /************************************************************************
    *              ASSET RELATED
    ************************************************************************/
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
      auto noAtlas = assets.atlas.isNull();
      auto noBackground = assets.background.isNull();
      auto noFont = assets.font.isNull();
      auto noMainVS = assets.mainVS.isNull();
      auto noMainPS = assets.mainPS.isNull();
      auto noHealthPS = assets.healthPS.isNull();
      auto noReload = assets.reload.isNull();
      auto noPistol = assets.pistol.isNull();
      auto noShotgun = assets.shotgun.isNull();
      auto noUzi = assets.uzi.isNull();
      auto noSniper = assets.sniper.isNull();
      auto noRpg = assets.rpg.isNull();

      auto result = noAtlas || noBackground || noFont ||
         noMainVS || noMainPS || noHealthPS ||
         noReload || noPistol || noShotgun || noUzi || noSniper || noRpg;

      return !result;
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

   /************************************************************************
    *              STATE RELATED
    ************************************************************************/
   core_internal void transitionState(Game* game)
   {
      if( game->nextState != game->currentState )
      {
         // #todo cleanup call for the current state before switching
         game->currentState = game->nextState;
         switch( game->currentState )
         {
            case State::MainMenu:
            {
               initState_mainMenu(game->mainMenuState, game->sharedData, game->constants, game->assets);
            } break;

            case State::GameplaySetup:
            {
               //result = init_gameplaySetup(game.gameplay, game.sharedData, game.constants, game.assets);
            } break;

            case State::GameplaySession:
            {
               //result = init_session(game, game.session);
            } break;
         }
      }
   }

   /************************************************************************
    *              GAME RELATED - GLOBAL
    ************************************************************************/
   core_internal Game* initGame(Memory mem, CommunicationBuffer* fromMain, CommunicationBuffer* toMain,
                                AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script)
   {
      Game* game = emplace<Game>(mem);
      game->gameMemory = mem;

      game->assets = loadGameAssets(audio, gfx);
      bool allLoaded = checkGameAssetsLoaded(game->assets);
      if( !allLoaded )
      {
         return nullptr;
      }

      game->currentState = State::Startup;
      game->nextState = State::MainMenu;

      game->isPaused = false;

      return game;
   }

   core_internal void shutdownGame(CommunicationBuffer* fromMain, CommunicationBuffer* toMain,
                                   AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                   Game* game)
   {
      unloadGameAssets(game->assets, audio, gfx);
   }

   core_internal bool tickLogic(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer,
                                AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                Game* game)
   {
      transitionState(game);

      WinMsg msg{};
      auto timeLimit = timer->getCurrentMicros();
      while( fromMain->peek(msg, timeLimit) )
      {
         switch( msg.type )
         {
            case WinMsgType::KeyboardKey:
            case WinMsgType::KeyboardText:
            case WinMsgType::MouseButton:
            case WinMsgType::MouseMove:
            case WinMsgType::MouseWheel:
            case WinMsgType::GamepadAxis:
            case WinMsgType::GamepadButton:
            case WinMsgType::GamepadConnection:
            {
               
            } break;
            case WinMsgType::FileChange:
            {
               onFileChange(msg, audio, gfx, input, script, game);
            } break;
            case WinMsgType::LostFocus:
            {
               onLostFocus(toMain, game);
            } break;
            case WinMsgType::GainFocus:
            {
               onGainFocus(toMain, game);
            } break;
            case WinMsgType::Close:
            {
               return false;
            } break;
         }
      }

      DeltaTime time{};
      time.real.micros = timer->getDeltaMicros();
      time.real.seconds = timer->getDeltaSeconds();
      time.virt.micros = game->isPaused ? 0 : time.real.micros;
      time.virt.seconds = game->isPaused ? 0 : time.real.seconds;

      switch( game->currentState )
      {
         case State::MainMenu:
         {
            //game->nextState = update_mainMenu(time, game.mainMenu, game.sharedData, game.constants, frameEvents, audio, lua, gfx);

            //skip state on button down or something.
         } break;
         case State::GameplaySetup:
         {
            //game->nextState = update_gameplaySetup(time, game.gameplay, game.sharedData, game.constants, frameEvents, audio, lua, gfx);

            //skip state on button down.
         } break;
         case State::GameplaySession:
         {
            //game.nextState = update_gameplaySession(time, game.constants, game.session, frameEvents, audio, lua, gfx, game.sharedData.camera);

            //skip state on button down
         } break;
         case State::Score:
         {
            //skip state on button down
         } break;
         case State::Quit:
         {
            return false;
         } break;
      }

      return true;
   }

   core_internal void tickRender(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer,
                                 AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script,
                                 Game* game)
   {

   }
}
