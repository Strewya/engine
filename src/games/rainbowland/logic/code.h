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
      HSound song;
   };

   struct Constants
   {
      float windowWidth;
      float windowHeight;
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

   struct CollisionShape
   {
      enum Type
      {
         PointShape,
         //LineShape,
         CircleShape,
         RectShape,
      };
      Type type;
      union
      {
         Vec2 point;
         //Line line???
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
   };

   struct CollisionPair
   {
      uint32_t collider;
      uint32_t collidee;
      Vec2 displacement;
   };

   struct SessionState
   {
      std::vector<DeltaTimeData> deltaTime;
      std::vector<RenderData> render;
      std::vector<PositionData> position;
      std::vector<MovementData> movement;
      std::vector<CollisionData> collision;
      std::vector<Vec2> targetDirection;
   };

   struct GameState
   {
      enum class GlobalGameState
      {
         Gameplay,
      };

      Constants constants;
      SessionState session;
      GlobalGameState globalGameState;
      Camera camera;
      GameResources assets;
      // #test
      FontDescriptor fontDesc;
      // #todo make this a mesh cache thingamabob
      std::vector<Mesh> meshes;

      bool musicPlaying;
   };
}
