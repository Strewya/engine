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
   public:
      struct Cell
      {
         std::vector<Monster*> contents;
      };
   
      std::vector<Cell> cells;
      Vec2f cellHalfsize;
      uint32_t columns;
      uint32_t rows;
   };

   Vec2i calculateCellCoords(Grid& grid, Vec2f position);
   Vec2f calculateCellPosition(Grid& grid, Vec2i cellCoords);
   int32_t indexFromCellCoords(Grid& grid, Vec2i cell);
   void updateMonsterInGrid(Grid& grid, Monster& monster);
   void removeMonsterFromGrid(Grid& grid, Monster& monster);
   void removeAllMonstersFromGrid(Grid& grid);
   std::vector<Monster*> collectMonstersInArea(Grid& grid, Vec2i centerCell, Vec2i box);
}
