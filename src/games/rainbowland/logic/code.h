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
      uint32_t deltaMicrosReal;
      uint32_t deltaMicrosVirt;
      float deltaTimeReal;
      float deltaTimeVirt;
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

   struct GameState
   {
      enum class GlobalGameState
      {
         MainMenu,
         Gameplay,
         Score
      };

      enum class GameplayState
      {
         //ColorPick,
         ClassPick,
         Session
      };

      Constants constants;
      SessionState session;
      GlobalGameState globalGameState;
      GameplayState gameplayState;
      Camera camera;
      GameResources assets;
      // #test
      FontDescriptor fontDesc;
      // #todo make this a mesh cache thingamabob
      std::vector<Mesh> meshes;
   };
}
