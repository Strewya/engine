#pragma once
/********************************************
*  contents:   declaration of systems used by the game
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct AudioSystem;
   struct FontSystem;
   struct GraphicsSystem;
   struct InputSystem;
   struct LuaSystem;

   struct GameSystems
   {
      AudioSystem* audio;
      FontSystem* font;
      GraphicsSystem* gfx;
      InputSystem* input;
      LuaSystem* lua;
   };
}
