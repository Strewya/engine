//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Grid.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Monster.h>
/******* end headers *******/

namespace Core
{
   Vec2i calculateCellCoords(Grid& grid, Vec2f position)
   {
      position += grid.cellHalfsize;

      position /= (grid.cellHalfsize * 2);

      position.x = std::floorf(position.x);
      position.y = std::floorf(position.y);

      return (Vec2i)position;
   }

   Vec2f calculateCellPosition(Grid& grid, Vec2i position)
   {
      return (Vec2f)position;
   }

   int32_t indexFromCellCoords(Grid& grid, Vec2i cell)
   {
      auto mx = grid.columns / 2;
      auto my = grid.rows / 2;
      cell.x += mx;
      cell.y += my;
      return cell.y*grid.columns + cell.x;
   }

   void updateMonsterInGrid(Grid& grid, Monster& monster)
   {
      auto monsterCell = calculateCellCoords(grid, monster.transform.position);
      auto oldCellIndex = indexFromCellCoords(grid, monster.gridCell);
      auto newCellIndex = indexFromCellCoords(grid, monsterCell);
      if( oldCellIndex != newCellIndex )
      {
         removeMonsterFromGrid(grid, monster);
         if( 0 <= newCellIndex && newCellIndex < (int32_t)grid.cells.size() )
         {
            grid.cells[newCellIndex].contents.emplace_back(&monster);
            monster.gridCell = monsterCell;
         }
      }
   }

   void removeMonsterFromGrid(Grid& grid, Monster& monster)
   {
      auto cellIndex = indexFromCellCoords(grid, monster.gridCell);
      if( 0 <= cellIndex && cellIndex < (int32_t)grid.cells.size() )
      {
         for( auto it = grid.cells[cellIndex].contents.begin();
             it != grid.cells[cellIndex].contents.end(); ++it )
         {
            if( *it == &monster )
            {
               *it = grid.cells[cellIndex].contents.back();
               grid.cells[cellIndex].contents.pop_back();
               break;
            }
         }
      }
   }

   void removeAllMonstersFromGrid(Grid& grid)
   {
      for( auto& cell : grid.cells )
      {
         cell.contents.clear();
      }
   }

   std::vector<Monster*> collectMonstersInArea(Grid& grid, Vec2i centerCell, Vec2i box)
   {
      std::vector<Monster*> result;
      for( int32_t y = centerCell.y - box.y; y <= (centerCell.y + box.y); ++y )
      {
         for( int32_t x = centerCell.x - box.x; x <= (centerCell.x + box.x); ++x )
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
