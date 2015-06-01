#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <unordered_map>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Entity;
   struct Vec2;

   struct MovementData
   {
   public:
      MovementData();

      void allocate(uint32_t newSize);
      void make(Entity e);
      void destroy(Entity e);

      uint32_t getEntitySlot(Entity e);

      Entity* _entity;
      float* _acceleration;
      Vec2* _velocity;
      Vec2* _direction;
      Vec2* _position;

      uint32_t _inUse;

   private:
      uint32_t m_allocated;
      std::vector<uint8_t> m_memory;
      std::unordered_map<uint32_t, uint32_t> m_map;

      uint32_t getDataIndex(uint32_t c);
      uint32_t makeComponent(uint32_t idx);
   };

   void doMovement(float dt, MovementData& data);
}
