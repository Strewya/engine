#pragma once
/********************************************
*	class:	Object rendering
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2Fwd.h>
/******* end header inclusion *******/

namespace Core
{
   class RainbowlandGame;
   struct Grid;
   class GraphicsSystem;

   void draw_splatters(RainbowlandGame& game, Vec2f atlasSize);
   void draw_background(RainbowlandGame& game, Vec2f atlasSize);
   void draw_players(RainbowlandGame& game, Vec2f atlasSize);
   void draw_monsters(RainbowlandGame& game, Vec2f atlasSize);
   void draw_pickups(RainbowlandGame& game, Vec2f atlasSize);
   void draw_skills(RainbowlandGame& game, Vec2f atlasSize);
   void draw_damagers(RainbowlandGame& game, Vec2f atlasSize);
   void draw_gui(RainbowlandGame& game, Vec2f atlasSize);
   void draw_grid_debug_info(Grid& grid, GraphicsSystem& gfx);
}
