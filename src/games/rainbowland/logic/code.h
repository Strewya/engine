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
#include "util/ring_buffer.h"
#include "util/transform.h"

#include "games/util/entity.h"
/******* end header inclusion *******/

namespace core
{
   static const bool Cursor_Lock = true;
   static const bool Cursor_Unlock = false;
   static const bool Cursor_Hide = false;
   static const bool Cursor_Show = true;
   static const bool Cursor_Absolute = false;
   static const bool Cursor_Relative = true;

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

   enum class MeshId;

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

   struct DataId
   {
      uint32_t id;
   };

   typedef std::function<void(Entity)> DestructionCallback;
   typedef std::vector<DestructionCallback> Callbacks;
   typedef std::unordered_map<Entity, DataId> EntityDataIdMap;
   typedef std::vector<Entity> EntityVector;

   struct EntityManager
   {
   public:
      EntityManager()
         : m_freeIds(32)
      {
         m_liveEntities.reserve(32);
      }
      Entity create()
      {
         Entity e{m_liveEntities.size()};
         if( m_freeIds.empty() )
         {
            m_liveEntities.push_back(true);
         }
         else
         {
            e.id = m_freeIds.front();
            m_freeIds.pop_front();
            m_liveEntities[e.id] = true;
         }
         return e;
      }
      void returnId(Entity e)
      {
         m_liveEntities[e.id] = false;
         m_freeIds.push_back(e.id);
      }
      bool isAlive(Entity e) const
      {
         auto result = m_liveEntities[e.id];
         return result;
      }
   private:
      RingBuffer m_freeIds;
      std::vector<bool> m_liveEntities;
   };

   struct DestructionNotifier
   {
   public:
      void registerCallback(DestructionCallback call)
      {
         m_registeredCallbacks.emplace_back(call);
      }
      void clear()
      {
         m_registeredCallbacks.clear();
      }
      void notifyDestruction(Entity e)
      {
         for( auto call : m_registeredCallbacks )
         {
            call(e);
         }
      }
      void notifyDestruction(const EntityVector& v)
      {
         if( !v.empty() && !m_registeredCallbacks.empty() )
         {
            for( auto call : m_registeredCallbacks )
            {
               for( auto e : v )
               {
                  call(e);
               }
            }
         }
      }
   private:
      Callbacks m_registeredCallbacks;
   };

   struct ComponentBase
   {
      EntityDataIdMap m_mapping;
      EntityVector m_entity;
   };

   struct ComponentDeltaTime : public ComponentBase
   {
      std::vector<uint32_t> m_deltaMicros;
      std::vector<float> m_deltaTime;
      std::vector<float> m_timeFactor;
   };

   struct ComponentMovement : public ComponentBase
   {
      std::vector<float> m_acceleration;
      std::vector<Vec2> m_direction;
      std::vector<Vec2> m_velocity;
      std::vector<Vec2> m_position;
   };

   struct ComponentTransform : public ComponentBase
   {
      std::vector<Vec2> m_position;
      std::vector<Vec2> m_scale;
      std::vector<float> m_rotation;
   };

   struct ComponentVisual : public ComponentBase
   {
      std::vector<Transform> m_transform;
      std::vector<Color> m_diffuse;
      std::vector<MeshId> m_meshId;
   };

   struct ComponentCollision : public ComponentBase
   {
      std::vector<CollisionShape> m_shape;
      std::vector<uint32_t> m_collisionGroup;
      std::vector<uint16_t> m_selfTypeBits;
      std::vector<uint16_t> m_targetTypeBits;
      std::vector<char> m_sensor;
      std::vector<char> m_previouslyColliding;
      std::vector<char> m_colliding;
   };

   struct World
   {
      EntityManager entityManager;
      DestructionNotifier destructionNotifier;

      ComponentDeltaTime deltaTime;
      ComponentTransform transform;
      ComponentMovement movement;
      ComponentCollision collision;
      ComponentVisual visual;
   };

   struct GameplayData
   {
      enum
      {
         BACK,
         COUNT
      };
      struct Numbers
      {
         enum
         {
            PlayersCount = 5,
            GearCount = 4,
         };
      };
      GuiData<COUNT> setupGui; // #temp because the gameplay setup part of the game should not have any visible buttons (i think)
      World world;
      Entity startArea;
      Entity players[Numbers::PlayersCount];
      Entity gear[Numbers::GearCount];
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
