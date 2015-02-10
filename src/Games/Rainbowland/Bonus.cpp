//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Bonus.h>
/******* C++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace Core
{
   Bonus& getBonus(VBonuses& list, BonusType id)
   {
      assert(id >= 0 && id < list.size());
      return list[id];
   }

   void enableBonus(Bonus& bonus)
   {
      if( bonus.acquireLogic )
      {
         bonus.acquireLogic();
      }
   }

   void updateBonus(Bonus& bonus)
   {
      if( bonus.timer.hasElapsed() )
      {
         disableBonus(bonus.type, game);
      }
   }

   void disableBonus(Bonus& bonus)
   {
      if( bonus.timeoutLogic )
      {
         bonus.timeoutLogic();
      }
   }
}
