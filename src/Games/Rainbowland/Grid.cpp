//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Grid.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Vec2i calculateCellCoords(Grid& grid, Vec2f position)
   {
      position.x += grid.cellHalfsize.x;
      position.y += grid.cellHalfsize.y;

      position.x /= (grid.cellHalfsize.x * 2);
      position.y /= (grid.cellHalfsize.y * 2);

      position.x = std::floorf(position.x);
      position.y = std::floorf(position.y);

      return (Vec2i)position;
   }

   int32_t indexFromCellCoords(Grid& grid, Vec2i cell)
   {
      auto mx = grid.columns / 2;
      auto my = grid.rows / 2;
      cell.x += mx;
      cell.y += my;
      return cell.y*grid.columns + cell.x;
   }

   void updateMonsterInGrid(Grid& grid, Monster& monster, Vec2i oldCell, Vec2i newCell)
   {
      auto oldCellIndex = indexFromCellCoords(grid, oldCell);
      auto newCellIndex = indexFromCellCoords(grid, newCell);
      if( oldCellIndex != newCellIndex )
      {
         removeMonsterfromGrid(grid, monster, oldCell);
         if( 0 <= newCellIndex && newCellIndex < (int32_t)grid.cells.size() )
         {
            grid.cells[newCellIndex].contents.emplace_back(&monster);
         }
      }
   }

   void removeMonsterfromGrid(Grid& grid, Monster& monster, Vec2i cell)
   {
      auto cellIndex = indexFromCellCoords(grid, cell);
      if( 0 <= cellIndex && cellIndex < (int32_t)grid.cells.size() )
      {
         for( auto*& mon : grid.cells[cellIndex].contents )
         {
            if( mon == &monster )
            {
               mon = grid.cells[cellIndex].contents.back();
               grid.cells[cellIndex].contents.pop_back();
               break;
            }
         }
      }
   }

   std::vector<Monster*> collectMonstersInArea(Grid& grid, Vec2i centerCell, Vec2i box)
   {
      std::vector<Monster*> result;
      for( int32_t y = centerCell.y - box.y; y <= centerCell.y + box.y; ++y )
      {
         for( int32_t x = centerCell.x - box.x; x <= centerCell.x + box.x; ++x )
         {
            Vec2i cell{x, y};
            auto index = indexFromCellCoords(grid, cell);
            if( 0 <= index && index < (int32_t)grid.cells.size() )
            {
               result.insert(result.end(), grid.cells[index].contents.begin(), grid.cells[index].contents.end());
            }
         }
      }
      return result;
   }
}
