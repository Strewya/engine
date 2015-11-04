#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/camera.h"
#include "graphics/font/font_descriptor.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   /************************************************************************
    *              ENUMS
    ************************************************************************/

   enum class State
   {
      MainMenu,
      GameplaySetup,
      GameplaySession,
      Score,
      Quit,
      Startup,
      Shutdown
   };

   enum Asset
   {
      AssetTextureAtlas = 0,
      AssetTextureBackground,
      AssetTextureFont,
      AssetTextureCount,
      AssetSoundReload = 0,
      AssetSoundPistol,
      AssetSoundShotgun,
      AssetSoundUzi,
      AssetSoundSniper,
      AssetSoundMissile,
      AssetSoundCount,
      AssetPixelShaderMain = 0,
      AssetPixelShaderHealth,
      AssetPixelShaderCount,
      AssetVertexShaderMain = 0,
      AssetVertexShaderCount,
      AssetMaterialWorld = 0,
      AssetMaterialFont,
      AssetMaterialHealth,
      AssetMaterialBackground,
      AssetMaterialCount,
      AssetFontMain = 0,
      AssetFontCount,
   };

   /************************************************************************
    *              GUI STRUCTS
    ************************************************************************/

   struct Button
   {
      u32 count;
      v2* position;
      v2* halfsize;
      // #todo at some point, these should be texture handles probably
      Color* idleColor;
      Color* hoverColor;
      Color* hotColor;
      const char** caption;
   };

   struct GuiSystem
   {
      Memory dynamicMemory;
      u32 hoverButton;
      u32 hotButton;
      u32 activatedButton;
      Button button;
   };

   /************************************************************************
    *              STRUCTS
    ************************************************************************/

   struct Constants
   {
      f32 windowWidth;
      f32 windowHeight;
   };

   struct GameAssets
   {
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

   struct CursorState
   {
      bool show;
      bool lock;
      bool relativeMovement;
   };

   struct SharedData
   {
      v2 mousePosition;
      Camera camera;
      // #temp This should be in a cache, and handles should be stored in assets.
      FontDescriptor font;
      //std::vector<Mesh> meshes;
      CursorState cursorState;
   };

   struct MainMenuData
   {
      enum
      {
         START_GAME,
         QUIT,
         COUNT
      };

      u32 buttonFunctionToExecute;
   };

   struct Game
   {
      Memory gameMemory;
      Memory dynamicMemory;
      GameAssets assets;

      State currentState;
      State nextState;

      bool isPaused;
      
      SharedData sharedData;
      Constants constants;

      GuiSystem gui;
   };

   struct Time
   {
      u32 micros;
      f32 seconds;
   };

   struct DeltaTime
   {
      Time real;
      Time virt;
   };
}
