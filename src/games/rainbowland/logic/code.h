#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
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

#include "games/util/entity.h"
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

   struct DeltaTimeComponentCache
   {
      std::vector<DeltaTimeData> m_data;
      std::unordered_map<Entity, InternalId> m_entityToInternalIdMap;
   };

   struct SharedData
   {
      Camera camera;
      // #temp This should be in a cache, and handles should be stored in assets.
      FontDescriptor font;
      std::vector<Mesh> meshes;

      bool showCursor;
      bool lockCursor;
      bool relativeCursor;
   };

   enum class State
   {
      MainMenu,
      GameplaySetup,
      GameplaySession,
      Score,
      Quit,
      Startup,
      Shutdown
   };

   enum ButtonState
   {
      IDLE,
      HOVER,
      HOT,
      COUNT
   };

   template<int COUNT>
   struct Button
   {
      std::array<Vec2, COUNT> position;
      std::array<Vec2, COUNT> halfsize;
      std::array<std::string, COUNT> caption;
   };

   template<int BUTTON_COUNT>
   struct GuiData
   {
      Vec2 mousePosition;
      uint32_t hoverButton;
      uint32_t hotButton;
      uint32_t activatedButton;
      Button<BUTTON_COUNT> button;
      std::array<Color, ButtonState::COUNT> buttonColors;
   };

   struct MainMenuState
   {
      enum
      {
         START_GAME,
         QUIT,
         COUNT
      };

      uint32_t buttonFunctionToExecute;
      GuiData<COUNT> gui;
   };

   struct Player
   {
      enum Color
      {
         BLUE,
         YELLOW,
         GREEN,
         CYAN,
         RED,
         COUNT
      };
   };

   struct GameplaySetupState
   {
      enum
      {
         BACK,
         COUNT
      };
      GuiData<COUNT> gui;
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
      State currentState;
      State nextState;

      GameResources assets;

      SharedData sharedData; //data that should be shared across ALL states
      Constants constants; //
      MainMenuState mainMenu;
      GameplaySetupState gameplaySetup;
      SessionState session;
      ScoreState score;
   };
}
