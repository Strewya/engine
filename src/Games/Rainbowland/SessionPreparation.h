#pragma once
/********************************************
*	class:	Session preparation
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class RainbowlandGame;

   void logic_sessionPreparation(RainbowlandGame& game);
   void render_sessionPreparation(RainbowlandGame& game);
   void initPreparationHandlers(RainbowlandGame& game);
}
