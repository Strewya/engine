#pragma once
/********************************************
*	class:	PhysicsSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <functional>
#include <vector>
/******* common headers *******/
#include <Box2D/Box2D.h>
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   typedef uint32_t InstanceID;

   class PhysicsSystem : public b2ContactListener
   {
   public:
      PhysicsSystem(); //needed because b2World doesn't have a default ctor

      bool init(const Vec2f& gravity, b2Draw* debugDraw = nullptr);
      bool shutdown();

      void update(float dt);
      void draw();

      void BeginContact(b2Contact* contact);
      void EndContact(b2Contact* contact);
      void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
      void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

      void addBeginContactListener(std::function<void(b2Contact*)> f);
      void addEndContactListener(std::function<void(b2Contact*)> f);

      InstanceID createBody(const b2BodyDef& bodyDef);
      b2Body* getBody(InstanceID bodyID) const;
      void destroyBody(InstanceID bodyID);

      InstanceID createFixture(const b2FixtureDef& fixtureDef, InstanceID bodyID);
      b2Fixture* getFixture(InstanceID fixtureID) const;
      void destroyFixture(InstanceID bodyID, uint32_t fixtureID);

      InstanceID createJoint(const b2JointDef& jointDef);
      b2Joint* getJoint(InstanceID jointID) const;
      void destroyJoint(InstanceID jointID);

   private:

      b2World m_world;

      std::vector<b2Body*> m_bodies;
      std::vector<b2Fixture*> m_fixtures;
      std::vector<b2Joint*> m_joints;

      std::vector<std::function<void(b2Contact*)>> m_beginContactListeners;
      std::vector<std::function<void(b2Contact*)>> m_endContactListeners;

      template<typename C, typename V, typename F> InstanceID store(C& container, V& data, F& cmp);
   };

   inline Vec2f convert(const b2Vec2& v)
   {
      return Vec2f(v.x, v.y);
   }

   inline b2Vec2 convert(const Vec2f& v)
   {
      return b2Vec2(v.x, v.y);
   }

   template<typename C, typename V, typename F> InstanceID PhysicsSystem::store(C& container, V& data, F& cmp)
   {
      auto it = std::find_if(std::begin(container), std::end(container), cmp);
      InstanceID id = 0;
      if( it == std::end(container) )
      {
         id = container.size();
         container.emplace_back(data);
      }
      else
      {
         id = std::distance(std::begin(container), it);
         *it = data;
      }
      return id;
   }
}
