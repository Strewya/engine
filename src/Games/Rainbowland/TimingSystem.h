#pragma once
/********************************************
*  contents:   Timing System
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Time.h>
/******* end header inclusion *******/

namespace Core
{
   class TimingSystem
   {
   public:

      void advanceTime(const Clock& clock)
      {
         for( auto& e : m_entries )
         {
            e.timer.updateBy(clock.getDeltaMicros());
         }
      }

      Timer& timer(uint32_t id)
      {
         for( auto& e : m_entries )
         {
            if( id == e.id )
            {
               return e.timer;
            }
         }
         m_entries.push_back({id, Timer{}});
         return m_entries.back().timer;
      }

   private:
      struct Entry
      {
         uint32_t id;
         Timer timer;
      };
      std::vector<Entry> m_entries;
   };
}
