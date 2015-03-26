#pragma once
/********************************************
*  contents:   Movement system prototype
*  usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/TimingSystem.h>
#include <Util/Utility.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class TimingSystem;

   struct Mover
   {
      Vec2f* position;
      Vec2f direction;
      float speed;
      float maxSpeed;
      float acceleration;
   };

   class MovementSystem
   {
   public:

      void move(TimingSystem& timingSystem)
      {
         for( auto& e : m_entries )
         {
            float dt = timingSystem.timer(e.id).getDeltaSeconds();
            e.mover.speed += e.mover.acceleration*dt;
            clamp(0.0f, e.mover.maxSpeed, e.mover.speed);
            auto velocity = e.mover.direction*e.mover.speed;
            *e.mover.position += (velocity*dt);
         }
      }

      Mover& mover(uint32_t id)
      {
         for( auto& e : m_entries )
         {
            if( id == e.id )
            {
               return e.mover;
            }
         }
         m_entries.push_back({id, Mover{}});
         return m_entries.back().mover;
      }

   private:
      struct Entry
      {
         uint32_t id;
         Mover mover;
      };
      std::vector<Entry> m_entries;
   };

}
