#pragma once
/********************************************
*  contents:   #todo
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class AudioSystem;
   class GraphicsSystem;
   class InputSystem;

   class GameSystems
   {
   public:
      AudioSystem* audio;
      GraphicsSystem* gfx;
      InputSystem* input;
   };
}
