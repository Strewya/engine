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
      //explicit Entity(uint32_t i) : id(i) {}
      uint32_t id;
   };

   inline bool operator==(Entity l, Entity r)
   {
      auto result = (l.id == r.id);

      return result;
   }

   inline bool operator<(Entity l, Entity r)
   {
      auto result = (l.id < r.id);

      return result;
   }

   inline bool operator!=(Entity l, Entity r)
   {
      auto result = !(l == r);

      return result;
   }

   inline std::ostream& operator<<(std::ostream& stream, Entity e)
   {
      stream << e.id;
      return stream;
   }
}

namespace std
{
   template <> struct hash < core::Entity >
   {
      size_t operator()(core::Entity e) const
      {
         return hash<int>()(e.id);
      }
   };
}