#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include "audio/fmod_sound_handle.h"
#include "graphics/camera.h"
#include "graphics/font/font_descriptor.h"
#include "graphics/mesh/mesh.h"
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_handle.h"
#include "util/geometry/circle.h"
#include "util/geometry/rect.h"
#include "util/geometry/vec2.h"
#include "util/color.h"
/******* end header inclusion *******/

namespace core
{
   struct GameResources
   {
      HTexture atlas;
      HTexture background;
      HTexture font;
      HVertexShader mainVS;
      HPixelShader mainPS;
      HPixelShader healthPS;
      HSound reload;
      HSound pistol;
      HSound uzi;
      HSound rpg;
      HSound sniper;
      HSound shotgun;
   };

   struct Constants
   {
      float windowWidth;
      float windowHeight;
      float playerAcceleration;
      float playerAimLength;
      bool showCursor;
      bool lockCursor;
      bool relativeCursor;
   };


   struct Time
   {
      uint32_t micros;
      float seconds;
   };

   struct DeltaTime
   {
      Time real;
      Time virt;
   };

   enum MeshId;

   struct DeltaTimeData
   {
      uint32_t deltaMicros;
      float deltaTime;
      float factor;
   };

   struct RenderData
   {
      Color color;
      MeshId mesh;
      float rotationRadians;
   };

   struct PositionData
   {
      Vec2 position;
   };

   struct MovementData
   {
      float acceleration;
      Vec2 direction;
      Vec2 velocity;
      Vec2 position;
   };

   struct Line
   {
      Vec2 lineFromOrigin;
   };

   struct CollisionShape
   {
      enum Type
      {
         PointShape,
         LineShape,
         CircleShape,
         RectShape,
      };
      Type type;
      union
      {
         Vec2 point;
         Line line;
         Circle circle;
         Rect rect;
      };
   };

   struct CollisionData
   {
      CollisionShape shape;
      uint32_t collisionGroup;
      uint16_t selfTypeBits;
      uint16_t targetTypeBits;
      bool sensor;
      bool previouslyInCollision;
      bool currentlyInCollision;
   };

   struct CollisionPair
   {
      uint32_t collider;
      uint32_t collidee;
      Vec2 displacement;
   };

   struct AimData
   {
      Vec2 aim;
   };

   struct InternalId
   {
      uint32_t id;
   };

   bool operator==(InternalId l, InternalId r);
   bool operator<(InternalId l, InternalId r);

   struct EntityId
   {
      uint32_t id;
   };

   inline bool operator==(EntityId l, EntityId r);
   inline bool operator<(EntityId l, EntityId r);
}

namespace std
{
   template <> struct hash < core::EntityId >
   {
      size_t operator()(core::EntityId id) const
      {
         return hash<int>()(id.id);
      }
   };
}

namespace core
{
   struct DeltaTimeComponentCache
   {
      std::vector<DeltaTimeData> m_data;
      std::unordered_map<EntityId, InternalId> m_entityToInternalIdMap;
   };

   struct SharedDataState
   {
      Camera camera;
   };

   struct MainMenuState
   {
      DeltaTimeComponentCache cTimer;
   };

   struct ClassPickState
   {

   };

   struct SessionState
   {
      uint32_t entityCount;
      std::vector<DeltaTimeData> deltaTime;
      std::vector<RenderData> render;
      std::vector<PositionData> position;
      std::vector<MovementData> movement;
      std::vector<AimData> aim;
      std::vector<CollisionData> collision;
      std::vector<Vec2> targetDirection;
   };

   struct ScoreState
   {

   };

   struct GameState
   {
      enum class State
      {
         MainMenu,
         Gameplay_ClassPick,
         Gameplay_Session,
         Score
      };

      State gameState;
      
      GameResources assets;
      // #temp These two should be in a cache, and handles should be stored in assets.
      FontDescriptor fontDesc;
      std::vector<Mesh> meshes;

      SharedDataState sharedData; //data that should be shared across ALL states
      Constants constants; //
      MainMenuState mainMenu;
      ClassPickState classPick;
      SessionState session;
      ScoreState score;
   };
}
