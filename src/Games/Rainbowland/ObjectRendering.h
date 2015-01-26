#pragma once
/********************************************
*	class:	Object rendering
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class RainbowlandGame;
   
   template<typename T> class Vec2;
   typedef Vec2<float> Vec2f;

   void draw_splatters_to_texture(RainbowlandGame& game, Vec2f atlasSize);
   void draw_background(RainbowlandGame& game, Vec2f atlasSize);
   void draw_players(RainbowlandGame& game, Vec2f atlasSize);
   void draw_monsters(RainbowlandGame& game, Vec2f atlasSize);
   void draw_pickups(RainbowlandGame& game, Vec2f atlasSize);
   void draw_skills(RainbowlandGame& game, Vec2f atlasSize);
   void draw_damagers(RainbowlandGame& game, Vec2f atlasSize);
   void draw_gui(RainbowlandGame& game, Vec2f atlasSize);
}
