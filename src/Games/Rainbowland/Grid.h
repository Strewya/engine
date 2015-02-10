#pragma once
/********************************************
*  contents:   Grid related stuff
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/MonsterFwd.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   struct Grid
   {
   private:
      struct Cell
      {
         std::vector<Monster*> contents;
      };
   public:
      std::vector<Cell> cells;
      Vec2f cellHalfsize;
      uint32_t columns;
      uint32_t rows;
   };

   Vec2i calculateCellCoords(Grid& grid, Vec2f position);
   int32_t indexFromCellCoords(Grid& grid, Vec2i cell);
   void updateMonsterInGrid(Grid& grid, Monster& monster, Vec2i oldCell, Vec2i newCell);
   void removeMonsterfromGrid(Grid& grid, Monster& monster, Vec2i cell);
   std::vector<Monster*> collectMonstersInArea(Grid& grid, Vec2i centerCell, Vec2i box);
}
