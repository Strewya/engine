#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{

   struct Entity
   {
      enum
      {
         INDEX_BITS = 24,
         INDEX_MASK = (1 << INDEX_BITS) - 1,
         GENERATION_MASK = ~INDEX_MASK
      };
      uint32_t id;

      uint32_t index()
      {
         auto result = id & INDEX_MASK;

         return result;
      }

      uint32_t generation()
      {
         auto result = (id & GENERATION_MASK) >> INDEX_BITS;

         return result;
      }
   };

   inline bool operator==(const Entity& l, const Entity& r)
   {
      auto result = l.id == r.id;

      return result;
   }

   inline bool operator<(const Entity& l, const Entity& r)
   {
      auto result = l.id < r.id;

      return result;
   }
}