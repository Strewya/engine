#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
#include <vector>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "audio/fmod_sound_handle.h"
#include "graphics/camera.h"
#include "graphics/font/font_descriptor.h"
#include "graphics/mesh/mesh.h"
#include "graphics/shader/shader_handle.h"
#include "graphics/texture/texture_handle.h"
#include "utility/geometry/circle.h"
#include "utility/geometry/rect.h"
#include "utility/geometry/vec2.h"
#include "utility/color.h"
#include "utility/ring_buffer.h"
#include "utility/transform.h"

#include "games/entity.h"
/******* end header inclusion *******/

namespace core
{
   enum
   {
      Cursor_Lock = true,
      Cursor_Unlock = false,
      Cursor_Hide = false,
      Cursor_Show = true,
      Cursor_Absolute = false,
      Cursor_Relative = true,
   };

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
      f32 windowWidth;
      f32 windowHeight;
      f32 playerAcceleration;
      f32 playerAimLength;
   };

   struct Time
   {
      u32 micros;
      f32 seconds;
   };

   struct DeltaTime
   {
      Time real;
      Time virt;
   };

   enum class MeshId;

   struct Line
   {
      v2 lineFromOrigin;
   };

   struct CollisionShape
   {
      enum class Type
      {
         PointShape,
         LineShape,
         CircleShape,
         RectShape,
      };
      Type type;
      union
      {
         v2 point;
         Line line;
         Circle circle;
         Rect rect;
      };
   };

   struct CollisionPair
   {
      Entity collider;
      Entity collidee;
      v2 displacement;
   };

   struct SharedData
   {
      v2 mousePosition;
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
      std::array<v2, COUNT> position;
      std::array<v2, COUNT> halfsize;
      // #todo at some point, these should be texture handles probably
      std::array<Color, COUNT> idleColor;
      std::array<Color, COUNT> hoverColor;
      std::array<Color, COUNT> hotColor;
      std::array<std::string, COUNT> caption;
   };

   template<int BUTTON_COUNT>
   struct GuiData
   {
      u32 hoverButton;
      u32 hotButton;
      u32 activatedButton;
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

      u32 buttonFunctionToExecute;
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
      u32 id;
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
      std::vector<u32> m_deltaMicros;
      std::vector<f32> m_deltaTime;
      std::vector<f32> m_timeFactor;
   };

   struct ComponentMovement : public ComponentBase
   {
      std::vector<f32> m_acceleration;
      std::vector<v2> m_turnDirection;
      std::vector<v2> m_direction;
      std::vector<v2> m_velocity;
      std::vector<v2> m_position;
   };

   struct ComponentTransform : public ComponentBase
   {
      std::vector<v2> m_position;
      std::vector<v2> m_scale;
      std::vector<f32> m_rotation;
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
      std::vector<u32> m_collisionGroup;
      std::vector<u16> m_selfTypeBits;
      std::vector<u16> m_targetTypeBits;
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
      struct GuiButton
      {
         enum
         {
            BACK,
            COUNT
         };
      };
      struct Numbers
      {
         enum
         {
            PlayersCount = 5,
            GearCount = 4,
         };
      };
      GuiData<GuiButton::COUNT> setupGui; // #temp because the gameplay setup part of the game should not have any visible buttons (i think)
      World world;
      Entity startArea;
      std::array<Entity, Numbers::PlayersCount> players;
      std::array<Entity, Numbers::GearCount> gear;
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
