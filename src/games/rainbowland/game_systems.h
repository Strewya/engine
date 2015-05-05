#pragma once
/********************************************
*  contents:   declaration of systems used by the game
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   struct AudioSystem;
   struct GraphicsSystem;
   struct InputSystem;

   struct GameSystems
   {
      AudioSystem* audio;
      GraphicsSystem* gfx;
      InputSystem* input;
   };
}
