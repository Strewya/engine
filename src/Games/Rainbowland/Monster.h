#pragma once
/********************************************
*  contents:   Monster related structures
*  usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
/******* common headers *******/
#include <Games/Rainbowland/MonsterFwd.h>
/******* extra headers *******/
#include <Util/Circle.h>
#include <Util/Color.h>
#include <Util/Time.h>
#include <Util/Transform.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   enum BrainState : int
   {
      BS_Wander,
      BS_Flank,
      BS_Attack
   };

   enum MonsterType : int
   {
      Flower,
      Ladybug,
      Butterfly,
      MonsterTypeCount
   };

   struct Brain
   {
      PeriodicTimer timer;
      BrainState state;

      uint32_t targetPlayer;
      Vec2f targetLocation;

      float attackRadius;
      float chanceToWander;
   };

   struct Monster
   {
      Brain brain;
      Timer objectTimer;
      Transform transform;
      Color color;
      Circle collisionData_attack;
      Circle collisionData_hitbox;
      Circle collisionData_separation;
      Vec2f direction;
      float maxSpeed;
      float turnSpeed;
      int32_t maxHealth;
      int32_t health;
      CooldownTimer attackTimer;
      uint32_t damage;
      uint32_t expGain;
      MonsterType type;
   };

   struct MonsterSpawner
   {
      PeriodicTimer spawnTimer;
      Transform transform;
      float spawnRadius;
   };
}
