//headers should be ordered alphabetically!
/******* precompiled header *******/
//#include "stdafx.h"
/******* personal header *******/
/******* c++ headers *******/
/******* extra headers *******/
#include "input/mouse.h"
#include "games/rainbowland/rainbowland.h"
#include "graphics/mesh/mesh.h"
#include "graphics/font/font_system.h"
#include "graphics/font/font_descriptor.h"
#include "utility/collision_checks.h"
#include "utility/color.h"
#include "utility/communication_buffer.h"
#include "utility/memory.h"
#include "utility/transform.h"
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

   core_internal void allocateGuiButtons(GuiSystem& gui, u32 buttonCount)
   {
      auto memory = gui.dynamicMemory;
      gui.button.count = buttonCount;
      gui.button.position = emplaceArray<v2>(memory, buttonCount);
      gui.button.halfsize = emplaceArray<v2>(memory, buttonCount);
      gui.button.idleColor = emplaceArray<Color>(memory, buttonCount);
      gui.button.hoverColor = emplaceArray<Color>(memory, buttonCount);
      gui.button.hotColor = emplaceArray<Color>(memory, buttonCount);
      gui.button.caption = emplaceArray<const char*>(memory, buttonCount);
   }

   inline v2 orthoMousePosition(MousePosition mouse, f32 windowWidth, f32 windowHeight)
   {
      v2 result;

      result.x = mouse.x - windowWidth / 2;
      result.y = -mouse.y + windowHeight / 2;

      return result;
   }

   core_internal void handleWinMsg_mouseMove(GuiSystem& gui, v2 mousePosition)
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

   core_internal void handleWinMsg_mouseDown(GuiSystem& gui)
   {
      gui.hotButton = gui.hoverButton;
   }

   core_internal u32 handleWinMsg_mouseUp(GuiSystem& gui)
   {
      u32 activateButton = gui.button.count;
      if( gui.hotButton == gui.hoverButton )
      {
         activateButton = gui.hotButton;
      }
      gui.hotButton = gui.button.count;
      return activateButton;
   }

   core_internal void handleInput_gui(GuiSystem& gui, SharedData& shared, const Constants& constants, WinMsgArray frameEvents)
   {
      gui.activatedButton = gui.button.count;
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
               handleWinMsg_mouseMove(gui, shared.mousePosition);
            } break;

            case WinMsgType::MouseButton:
            {
               if( event.mouse.button.id == Mouse::LeftButton )
               {
                  if( event.mouse.button.isDown )
                  {
                     handleWinMsg_mouseDown(gui);
                  }
                  else
                  {
                     gui.activatedButton = handleWinMsg_mouseUp(gui);
                  }
               }
            } break;
         }
      }
   }

   core_internal void render_gui(GuiSystem& gui, SharedData& shared, const GameAssets& assets, GraphicsSystem* gfx)
   {
      gfx->setOrthographicProjection();

      Material m;
      //m.pixelShaderHandle = assets.mainPS;
      //m.vertexShaderHandle = assets.mainVS;
      //m.textureHandle = assets.font;

      for( auto i = 0U; i < gui.button.count; ++i )
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

         auto buttonFrameMesh = makeMesh_outlinedQuad(gfx, {}, gui.button.halfsize[i]);
         auto buttonTextMesh = makeMesh_text(gfx, gui.button.caption[i], shared.font, {1, 1}, TextJustification::Center, TextJustification::Middle);

         gfx->renderMesh(buttonTransform, color, buttonFrameMesh, m);
         gfx->renderMesh(buttonTransform, {}, buttonTextMesh, m);
      }

      auto cursorMesh = makeMesh_solidCircle(gfx, {}, 3, 16);
      gfx->renderMesh({shared.mousePosition}, {}, cursorMesh, m);

   }

   /************************************************************************
    *              MAIN MENU RELATED
    ************************************************************************/
   core_internal void initState_mainMenu(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {
      allocateGuiButtons(game->gui, MainMenuData::COUNT);

      game->gui.hoverButton = MainMenuData::COUNT;
      game->gui.hotButton = MainMenuData::COUNT;
      game->gui.activatedButton = MainMenuData::COUNT;

      Color defaultIdleColor{1, 1, 1};
      Color defaultHoverColor{1, 1, 0};
      Color defaultHotColor{1, 0, 0};

      auto i = 0U;
      game->gui.button.position[i++] = {0, 100};
      game->gui.button.position[i++] = {0, -100};

      i = 0;
      game->gui.button.halfsize[i++] = {200, 50};
      game->gui.button.halfsize[i++] = {200, 50};

      i = 0;
      game->gui.button.idleColor[i++] = {0, 1, 0};
      game->gui.button.idleColor[i++] = {0, 1, 1};

      i = 0;
      game->gui.button.hoverColor[i++] = defaultHoverColor;
      game->gui.button.hoverColor[i++] = defaultHoverColor;

      i = 0;
      game->gui.button.hotColor[i++] = defaultHotColor;
      game->gui.button.hotColor[i++] = defaultHotColor;

      i = 0;
      game->gui.button.caption[i++] = "Start game";
      game->gui.button.caption[i++] = "Quit";
   }

   core_internal void updateState_mainMenu(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {
      auto frameEvents = input->getMessageArray();
      handleInput_gui(game->gui, game->sharedData, game->constants, frameEvents);
   }

   core_internal void renderState_mainMenu(GraphicsSystem* gfx, Game* game)
   {
      gfx->applyCamera(game->sharedData.camera);

      render_gui(game->gui, game->sharedData, game->assets, gfx);
   }

   core_internal void cleanState_mainMenu(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {

   }

   /************************************************************************
    *              ASSET RELATED
    ************************************************************************/
   core_internal void loadGameAssets(AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* lua, Game* game)
   {
      Memory stringBuffer = game->dynamicMemory;
      auto tocFile = script::openConfigFile(lua, CORE_RESOURCE("toc.lua"));
      if( tocFile.index )
      {
         auto block = script::openNamespace(lua, tocFile, "textures");
         game->assets.textures = nullptr; //this line should rather setup a default texture
         game->assets.numTextures = 0;
         if( block.index )
         {
            for( auto i = 0U; i < AssetTextureCount; ++i )
            {
               str path = script::readIndexedValue(lua, block, i, "", stringBuffer);
               auto handle = gfx->loadTextureFromFile(path);
               if( !handle.isNull() )
               {
                  auto* ptr = emplace<HTexture>(game->gameMemory);
                  *ptr = handle;
                  ++game->assets.numTextures;
                  if( game->assets.textures == nullptr )
                  {
                     game->assets.textures = ptr;
                  }
               }
            }
            script::closeNamespace(lua, block);
         }
         block = script::openNamespace(lua, tocFile, "sounds");
         game->assets.sounds = nullptr;
         game->assets.numSounds = 0;
         if( block.index )
         {
            for( auto i = 0U; i < AssetSoundCount; ++i )
            {
               str path = script::readIndexedValue(lua, block, i, "", stringBuffer);
               auto handle = audio->loadSoundFromFile(path);
               if( !handle.isNull() )
               {
                  auto* ptr = emplace<HSound>(game->gameMemory);
                  *ptr = handle;
                  ++game->assets.numSounds;
                  if( game->assets.sounds == nullptr )
                  {
                     game->assets.sounds = ptr;
                  }
               }
            }
            script::closeNamespace(lua, block);
         }
         block = script::openNamespace(lua, tocFile, "");
      }
      script::closeConfigFile(lua, tocFile);

      // sounds - standalone
      // textures - standalone
      // shaders - standalone
      // mesh - standalone
      // materials - requires texture, shader
      // sprites - requires mesh, material
      // fonts - requires material
   }

   core_internal bool checkGameAssetsLoaded(GameAssets& assets)
   {
      auto notLoaded = false;

      for( auto i = 0U; i < assets.numTextures; ++i )
      {
         notLoaded = notLoaded || assets.textures[i].isNull();
      }
      for( auto i = 0U; i < assets.numSounds; ++i )
      {
         notLoaded = notLoaded || assets.sounds[i].isNull();
      }
      
      return notLoaded;
   }

   core_internal void unloadGameAssets(GameAssets& assets, AudioSystem* audio, GraphicsSystem* gfx)
   {
      for( auto i = 0U; i < assets.numTextures; ++i )
      {
         gfx->unload(assets.textures[i]);
      }
      for( auto i = 0U; i < assets.numSounds; ++i )
      {
         audio->unload(assets.sounds[i]);
      }
   }

   /************************************************************************
    *              STATE RELATED
    ************************************************************************/

   core_internal void initState(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {
      switch( game->currentState )
      {
         case State::MainMenu:
         {
            initState_mainMenu(audio, gfx, input, script, game);
         } break;

         default:
         {
            CORE_LOG_DEBUG("State not implemented yet.");
         } break;
      }
   }
   core_internal void updateState(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {
      switch( game->currentState )
      {
         case State::MainMenu:
         {
            updateState_mainMenu(audio, gfx, input, script, game);
         } break;

         default:
         {
            CORE_LOG_DEBUG("State not implemented yet.");
         } break;
      }
   }
   core_internal void renderState(GraphicsSystem* gfx, Game* game)
   {
      switch( game->currentState )
      {
         case State::MainMenu:
         {
            renderState_mainMenu(gfx, game);
         } break;

         default:
         {
            CORE_LOG_DEBUG("State not implemented yet.");
         } break;
      }
   }
   core_internal void cleanState(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script, Game* game)
   {
      switch( game->currentState )
      {
         case State::MainMenu:
         {
            cleanState_mainMenu(audio, gfx, input, script, game);
         } break;

         default:
         {
            CORE_LOG_DEBUG("State not implemented yet.");
         } break;
      }
   }
   core_internal bool transitionState(AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                      Game* game)
   {
      if( game->currentState != game->nextState )
      {
         cleanState(audio, gfx, input, script, game);
         game->currentState = game->nextState;
         initState(audio, gfx, input, script, game);
      }
      return game->currentState != State::Quit;
   }

   /************************************************************************
    *              GAME RELATED - GLOBAL
    ************************************************************************/

   core_internal void registerAssetValues(LuaSystem* L)
   {
      auto luaGlobalNamespace = script::getGlobalNamespace(L);
      auto luaAssetsNamespace = script::beginNewNamespace(L, "AssetID");

#define RegisterNumber(name) script::writeNamedValue(L, luaAssetsNamespace, #name, name)

      RegisterNumber(AssetSoundReload);
      RegisterNumber(AssetSoundPistol);
      RegisterNumber(AssetSoundShotgun);
      RegisterNumber(AssetSoundUzi);
      RegisterNumber(AssetSoundSniper);
      RegisterNumber(AssetSoundMissile);

      RegisterNumber(AssetTextureAtlas);
      RegisterNumber(AssetTextureBackground);
      RegisterNumber(AssetTextureFont);

      RegisterNumber(AssetPixelShaderHealth);
      RegisterNumber(AssetPixelShaderMain);

      RegisterNumber(AssetVertexShaderMain);

      RegisterNumber(AssetFontMain);

      RegisterNumber(AssetMaterialBackground);
      RegisterNumber(AssetMaterialFont);
      RegisterNumber(AssetMaterialHealth);
      RegisterNumber(AssetMaterialWorld);

#undef RegisterNumber

      script::endNewNamespace(L, luaGlobalNamespace);
   }

   core_internal Game* initGame(Memory mem, CommunicationBuffer* fromMain, CommunicationBuffer* toMain,
                                AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* lua)
   {
      Game* game = emplace<Game>(mem);
      CORE_ASSERT_DBGERR(game != nullptr, "Not enough memory for core Game object.");

      game->gameMemory = mem;
      game->dynamicMemory = allocateMemoryChunk(game->gameMemory, MegaBytes(1), 16);

      registerAssetValues(lua);

      loadGameAssets(audio, gfx, lua, game);
      bool allLoaded = checkGameAssetsLoaded(game->assets);
      if( !allLoaded )
      {
         return nullptr;
      }

      game->gui.dynamicMemory = allocateMemoryChunk(game->gameMemory, MegaBytes(5), 16);

      game->currentState = game->nextState = State::MainMenu;
      game->isPaused = false;

      game->sharedData.camera.setPosition({0, 0, -50});

      initState(audio, gfx, input, lua, game);

      return game;
   }

   core_internal void shutdownGame(CommunicationBuffer* fromMain, CommunicationBuffer* toMain,
                                   AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                   Game* game)
   {
      cleanState(audio, gfx, input, script, game);
      unloadGameAssets(game->assets, audio, gfx);
   }

   core_internal bool tickLogic(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer,
                                AudioSystem* audio, GraphicsSystem* gfx, InputSystem* input, LuaSystem* script,
                                Game* game)
   {
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
               input->insert(msg);
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

      updateState(audio, gfx, input, script, game);

      input->clear();

      return transitionState(audio, gfx, input, script, game);
   }

   core_internal void tickRender(CommunicationBuffer* fromMain, CommunicationBuffer* toMain, Clock* timer,
                                 AudioSystem* audio, GraphicsSystem* gfx, LuaSystem* script,
                                 Game* game)
   {
      gfx->begin();

      renderState(gfx, game);

      gfx->present();
   }
}
