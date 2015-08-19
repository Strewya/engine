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

   struct SharedData
   {
      Vec2 mousePosition;
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
      // #todo at some point, these should be texture handles probably
      std::array<Color, COUNT> idleColor;
      std::array<Color, COUNT> hoverColor;
      std::array<Color, COUNT> hotColor;
      std::array<std::string, COUNT> caption;
   };

   template<int BUTTON_COUNT>
   struct GuiData
   {
      uint32_t hoverButton;
      uint32_t hotButton;
      uint32_t activatedButton;
      Button<BUTTON_COUNT> button;
   };

   struct MainMenuData
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

   typedef std::unordered_map<Entity, InternalId> EntityInternalIdMap;

   struct ComponentDeltaTime
   {
      std::vector<DeltaTimeData> m_data;
      EntityInternalIdMap m_entityToInternalIdMap;
   };

   struct InstanceProvider
   {
   public:
      InstanceProvider() : m_idCounter(0)
      {
      }
      Entity requestId()
      {
         return{++m_idCounter};
      }
      void returnId(Entity id)
      {
         //nothing for now...
      }
   private:
      uint32_t m_idCounter;
   };

   struct World
   {
      InstanceProvider instanceProvider;
      ComponentDeltaTime deltaTime;
   };

   struct GameplayData
   {
      enum
      {
         BACK,
         COUNT
      };
      GuiData<COUNT> setupGui; // #temp because the gameplay setup part of the game should not have any visible buttons (i think)
      World world;
   };

   struct GameData
   {
      State currentState;
      State nextState;

      GameResources assets;

      SharedData sharedData; //data that should be shared across ALL states
      Constants constants;
      MainMenuData mainMenu;
      GameplayData gameplay;
   };
}
