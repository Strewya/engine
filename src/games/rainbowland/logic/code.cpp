//headers should be ordered alphabetically!
/******* precompiled header *******/
#ifdef CORE_USE_PCH
#undef CORE_USE_PCH
#include "stdafx.h"
#define CORE_USE_PCH
#endif
/******* personal header *******/
#include "games/rainbowland/logic/code.h"
/******* c++ headers *******/
#include <functional>
#include <unordered_set>
/******* extra headers *******/
#include "input/gamepad.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "utility/collision_checks.h"
#include "utility/types.h"
/******* end headers *******/

namespace core
{
   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  globals
   //******************************************************************************************************************
   //******************************************************************************************************************

   core_global_constant DataId InvalidDataId{(std::numeric_limits<u32>::max())};

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  enums
   //******************************************************************************************************************
   //******************************************************************************************************************
   enum class MeshId
   {
      StartArea,
      KeyboardPlayer,
      MeshCount
   };

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  utility code
   //******************************************************************************************************************
   //******************************************************************************************************************

   inline bool operator==(DataId l, DataId r)
   {
      auto result = l.id == r.id;
      return result;
   }
   inline bool operator!=(DataId l, DataId r)
   {
      auto result = !(l == r);
      return result;
   }

   inline f32 pow2(f32 v)
   {
      auto result = v*v;
      return result;
   }

   inline f32 radians(Vec2 v)
   {
      auto result = std::atan2(v.y, v.x);
      return result;
   }

   inline Vec2 orthoMousePosition(MousePosition mouse, f32 windowWidth, f32 windowHeight)
   {
      Vec2 result;

      result.x = mouse.x - windowWidth / 2;
      result.y = -mouse.y + windowHeight / 2;

      return result;
   }

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  collision detection code
   //******************************************************************************************************************
   //******************************************************************************************************************

   template<typename T>
   core_internal CollisionResult areInCollision(T one, CollisionShape two)
   {
      CollisionResult result{};

      switch( two.type )
      {
         case CollisionShape::Type::PointShape:
         {
            result = checkCollision(one, two.point);
         } break;
         case CollisionShape::Type::CircleShape:
         {
            result = checkCollision(one, two.circle);
         } break;
         case CollisionShape::Type::RectShape:
         {
            result = checkCollision(one, two.rect);
         } break;
      }

      return result;
   }

   core_internal CollisionResult areInCollision(CollisionShape one, CollisionShape two)
   {
      CollisionResult result{};

      switch( one.type )
      {
         case CollisionShape::Type::PointShape:
         {
            result = areInCollision(one.point, two);
         } break;
         case CollisionShape::Type::CircleShape:
         {
            result = areInCollision(one.circle, two);
         } break;
         case CollisionShape::Type::RectShape:
         {
            result = areInCollision(one.rect, two);
         } break;
      }

      return result;
   }

   template<typename T>
   core_internal bool fullyContained(T one, CollisionShape two)
   {
      auto result = false;
      switch( two.type )
      {
         case CollisionShape::Type::PointShape:
         {
            result = isFullyWithin(one, two.point);
         } break;
         case CollisionShape::Type::CircleShape:
         {
            result = isFullyWithin(one, two.circle);
         } break;
         case CollisionShape::Type::RectShape:
         {
            result = isFullyWithin(one, two.rect);
         } break;
      }

      return result;
   }

   core_internal bool fullyContained(CollisionShape one, CollisionShape two)
   {
      auto result = false;
      switch( one.type )
      {
         case CollisionShape::Type::PointShape:
         {
            result = fullyContained(one.point, two);
         } break;
         case CollisionShape::Type::CircleShape:
         {
            result = fullyContained(one.circle, two);
         } break;
         case CollisionShape::Type::RectShape:
         {
            result = fullyContained(one.rect, two);
         } break;
      }
      return result;
   }

   core_internal bool shouldCollide(u32 group1, u32 group2, u16 selfType1, u16 selfType2, u16 targetType1, u16 targetType2)
   {
      if( group1 == group2 && group1 != 0 )
      {
         return group1 > 0;
      }

      bool shouldCollide = (selfType1 & targetType2) != 0 &&
         (selfType2 & targetType1) != 0;

      return shouldCollide;
   }

   core_internal void setCollisionShapePosition(CollisionShape& shape, Vec2 position)
   {
      switch( shape.type )
      {
         case CollisionShape::Type::RectShape:
         {
            shape.rect.center += position;
         } break;
         case CollisionShape::Type::CircleShape:
         {
            shape.circle.center += position;
         } break;
         case CollisionShape::Type::PointShape:
         {
            shape.point += position;
         } break;
      }
   }

   core_internal void setCollisionShapeScale(CollisionShape& shape, Vec2 scale)
   {
      switch( shape.type )
      {
         case CollisionShape::Type::RectShape:
         {
            shape.rect.halfSize *= scale;
         } break;
         case CollisionShape::Type::CircleShape:
         {
            shape.circle.radius *= scale.x;
         } break;
      }
   }



   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  ECS code
   //******************************************************************************************************************
   //******************************************************************************************************************
   namespace component_impl
   {
      template<typename DATA>
      core_internal void requestSlot(DATA& data, DataId i, typename DATA::value_type defaultValue = {})
      {
         if( data.size() == i.id )
         {
            data.emplace_back(defaultValue);
         }
         else
         {
            data[i.id] = defaultValue;
         }
      }

      core_internal DataId requestSlot(ComponentBase& cmp, Entity e)
      {
         DataId id{cmp.m_mapping.size()};
         cmp.m_mapping.insert({e, id});
         requestSlot(cmp.m_entity, id, e);
         return id;
      }

      template<typename DATA>
      core_internal void releaseSlot(DATA& data, std::pair<DataId, DataId> ids)
      {
         if( ids.first != InvalidDataId && ids.second != InvalidDataId )
         {
            std::swap(data[ids.first.id], data[ids.second.id]);
         }
      }

      core_internal std::pair<DataId, DataId> releaseSlot(ComponentBase& cmp, Entity e)
      {
         std::pair<DataId, DataId> result{InvalidDataId, InvalidDataId};
         auto it = cmp.m_mapping.find(e);
         if( it != std::end(cmp.m_mapping) )
         {
            auto last = cmp.m_mapping.size() - 1;
            result.first = it->second;
            result.second = {last};
            auto lastIt = std::find_if(std::begin(cmp.m_mapping), std::end(cmp.m_mapping),
                                       [last](EntityDataIdMap::value_type& v)
            {
               auto result = (v.second.id == last);
               return result;
            });
            lastIt->second = it->second;
            cmp.m_mapping.erase(it);
            releaseSlot(cmp.m_entity, result);
         }
         return result;
      }
   }
   //*******************************************************
   //********************* GET DATA ID *********************
   //*******************************************************
   core_internal DataId getDataId(const ComponentBase& cache, Entity e)
   {
      auto result = InvalidDataId;
      auto it = cache.m_mapping.find(e);
      if( it != std::end(cache.m_mapping) )
      {
         result = it->second;
      }
      return result;
   }
   //*********************************************************
   //********************* CONTAINS DATA *********************
   //*********************************************************
   core_internal bool containsData(const ComponentBase& cache, Entity e)
   {
      auto id = getDataId(cache, e);
      auto result = (id != InvalidDataId);
      return result;
   }
   //*************************************************
   //********************* CLEAR *********************
   //*************************************************
   core_internal void clearAll(ComponentBase& cache)
   {
      cache.m_mapping.clear();
   }
   //*****************************************************
   //********************* GET COUNT *********************
   //*****************************************************
   core_internal u32 getCount(const ComponentBase& cache)
   {
      auto result = cache.m_mapping.size();
      return result;
   }
   //*******************************************************
   //********************* CREATE DATA *********************
   //*******************************************************
   core_internal DataId createData(ComponentDeltaTime& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_deltaMicros, iid);
      component_impl::requestSlot(cache.m_deltaTime, iid);
      component_impl::requestSlot(cache.m_timeFactor, iid, 1.0f);
      return iid;
   }
   core_internal DataId createData(ComponentMovement& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_acceleration, iid);
      component_impl::requestSlot(cache.m_turnDirection, iid);
      component_impl::requestSlot(cache.m_direction, iid);
      component_impl::requestSlot(cache.m_velocity, iid);
      component_impl::requestSlot(cache.m_position, iid);
      return iid;
   }
   core_internal DataId createData(ComponentTransform& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_position, iid);
      component_impl::requestSlot(cache.m_scale, iid, {1, 1});
      component_impl::requestSlot(cache.m_rotation, iid);
      return iid;
   }
   core_internal DataId createData(ComponentVisual& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_transform, iid);
      component_impl::requestSlot(cache.m_diffuse, iid);
      component_impl::requestSlot(cache.m_meshId, iid);
      return iid;
   }
   core_internal DataId createData(ComponentCollision& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_shape, iid);
      component_impl::requestSlot(cache.m_collisionGroup, iid, 1);
      component_impl::requestSlot(cache.m_selfTypeBits, iid);
      component_impl::requestSlot(cache.m_targetTypeBits, iid);
      component_impl::requestSlot(cache.m_sensor, iid);
      component_impl::requestSlot(cache.m_previouslyColliding, iid);
      component_impl::requestSlot(cache.m_colliding, iid);
      return iid;
   }
   //********************************************************
   //********************* RELEASE DATA *********************
   //********************************************************
   core_internal void releaseData(ComponentDeltaTime& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_deltaMicros, iid);
      component_impl::releaseSlot(cache.m_deltaTime, iid);
      component_impl::releaseSlot(cache.m_timeFactor, iid);
   }
   core_internal void releaseData(ComponentMovement& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_acceleration, iid);
      component_impl::releaseSlot(cache.m_turnDirection, iid);
      component_impl::releaseSlot(cache.m_direction, iid);
      component_impl::releaseSlot(cache.m_velocity, iid);
      component_impl::releaseSlot(cache.m_position, iid);
   }
   core_internal void releaseData(ComponentTransform& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_position, iid);
      component_impl::releaseSlot(cache.m_scale, iid);
      component_impl::releaseSlot(cache.m_rotation, iid);
   }
   core_internal void releaseData(ComponentVisual& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_transform, iid);
      component_impl::releaseSlot(cache.m_diffuse, iid);
      component_impl::releaseSlot(cache.m_meshId, iid);
   }
   core_internal void releaseData(ComponentCollision& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_shape, iid);
      component_impl::releaseSlot(cache.m_collisionGroup, iid);
      component_impl::releaseSlot(cache.m_selfTypeBits, iid);
      component_impl::releaseSlot(cache.m_targetTypeBits, iid);
      component_impl::releaseSlot(cache.m_sensor, iid);
      component_impl::releaseSlot(cache.m_previouslyColliding, iid);
      component_impl::releaseSlot(cache.m_colliding, iid);
   }
   //*********************************************************************
   //********************* MAKE DESTRUCTION CALLBACK *********************
   //*********************************************************************
   template<typename COMPONENT>
   core_internal DestructionCallback makeDestructionCallback(COMPONENT& cmp)
   {
      return std::bind<void(COMPONENT&, Entity)>(releaseData, std::ref(cmp), std::placeholders::_1);
   }
   //************************************************
   //********************* READ *********************
   //************************************************
#define forbid(storage, field)
#define getter(storage, field) \
core_internal auto read_##field(const storage& component, DataId id) -> std::remove_reference<decltype(component.m_##field[id.id])>::type { return component.m_##field[id.id]; }
   //************************************************
   getter(ComponentDeltaTime, timeFactor);
   getter(ComponentDeltaTime, deltaTime);
   getter(ComponentDeltaTime, deltaMicros);
   getter(ComponentMovement, acceleration);
   getter(ComponentMovement, velocity);
   getter(ComponentMovement, direction);
   getter(ComponentMovement, position);
   getter(ComponentTransform, position);
   getter(ComponentTransform, scale);
   getter(ComponentTransform, rotation);
   forbid(ComponentVisual, transform);
   forbid(ComponentVisual, diffuse);
   forbid(ComponentVisual, meshId);
   getter(ComponentCollision, shape);
   getter(ComponentCollision, collisionGroup);
   getter(ComponentCollision, selfTypeBits);
   getter(ComponentCollision, targetTypeBits);
   getter(ComponentCollision, sensor);
   getter(ComponentCollision, previouslyColliding);
   getter(ComponentCollision, colliding);
#undef getter
      //*************************************************
      //********************* WRITE *********************
      //************************************************
#define setter(storage, field) \
core_internal void write_##field(storage& component, DataId id, std::remove_reference<decltype(component.m_##field[id.id])>::type value) { component.m_##field[id.id] = value; }
      //*************************************************
   setter(ComponentDeltaTime, timeFactor);
   forbid(ComponentDeltaTime, deltaTime);
   forbid(ComponentDeltaTime, deltaMicros);
   setter(ComponentMovement, acceleration);
   setter(ComponentMovement, velocity);
   setter(ComponentMovement, direction);
   setter(ComponentMovement, position);
   setter(ComponentTransform, position);
   setter(ComponentTransform, scale);
   setter(ComponentTransform, rotation);
   setter(ComponentVisual, transform);
   setter(ComponentVisual, diffuse);
   setter(ComponentVisual, meshId);
   setter(ComponentCollision, shape);
   setter(ComponentCollision, collisionGroup);
   setter(ComponentCollision, selfTypeBits);
   setter(ComponentCollision, targetTypeBits);
   setter(ComponentCollision, sensor);
   setter(ComponentCollision, previouslyColliding);
   setter(ComponentCollision, colliding);
#undef setter
#undef not_available
      //*************************************************************
      //********************* SYSTEM OPERATIONS *********************
      //*************************************************************
   core_internal void advanceTimeForEntities(ComponentDeltaTime& cache, Time dt)
   {
      auto count = getCount(cache);
      for( auto i = 0U; i < count; ++i )
      {
         auto factor = cache.m_timeFactor[i];
         cache.m_deltaMicros[i] = static_cast<u32>(dt.micros*factor);
         cache.m_deltaTime[i] = dt.seconds*factor;
      }
   }
   core_internal void turnEntities(ComponentMovement& movement, ComponentDeltaTime& deltaTime)
   {
      auto count = getCount(movement);
      for( auto i = 0U; i < count; ++i )
      {
         auto target = movement.m_turnDirection[i];
         if( !vec2::isZero(target) && movement.m_acceleration[i] > 0.0f )
         {
            f32 currentRad = radians(movement.m_direction[i]);
            f32 targetRad = radians(vec2::normalize(target));
            f32 currentDeg = Rad2Deg(currentRad);
            f32 targetDeg = Rad2Deg(targetRad);

            auto wra = targetDeg - currentDeg;
            if( wra > 180.0f ) wra -= 360.0f;
            if( wra < -180.0f ) wra += 360.0f;

            //this 4 should be turn speed or something
            auto iid = getDataId(deltaTime, movement.m_entity[i]);
            currentDeg += wra * 4 * read_deltaTime(deltaTime, iid);
            currentRad = Deg2Rad(currentDeg);

            movement.m_direction[i].x = std::cosf(currentRad);
            movement.m_direction[i].y = std::sinf(currentRad);
         }
      }
   }
   core_internal void moveEntities(ComponentMovement& movement, const ComponentDeltaTime& deltaTimes)
   {
      auto count = movement.m_mapping.size();
      for( auto i = 0U; i < count; ++i )
      {
         auto e = movement.m_entity[i];
         auto iid = getDataId(deltaTimes, e);
         CORE_ASSERT_DEBUG(iid != InvalidDataId, "Moveable entity [id=", e, "] lacks a DeltaTime component!");
         auto dt = read_deltaTime(deltaTimes, iid);
         auto acceleration = movement.m_direction[i] * movement.m_acceleration[i];
         acceleration += -movement.m_velocity[i] * 10.0f;
         movement.m_position[i] = acceleration*0.5f*pow2(dt) + movement.m_velocity[i] * dt + movement.m_position[i];
         movement.m_velocity[i] = acceleration*dt + movement.m_velocity[i];
      }
   }
   core_internal void syncPositionAfterMovement(const ComponentMovement& movement, ComponentTransform& transform)
   {
      auto count = getCount(movement);
      for( auto i = 0U; i < count; ++i )
      {
         auto entity = movement.m_entity[i];
         auto iid = getDataId(transform, entity);
         write_position(transform, iid, movement.m_position[i]);
      }
   }
   core_internal void syncPositionForRendering(const ComponentTransform& transform, ComponentVisual& visual)
   {
      auto count = getCount(transform);
      for( auto i = 0U; i < count; ++i )
      {
         auto entity = transform.m_entity[i];
         auto iid = getDataId(visual, entity);
         Transform tf{transform.m_position[i], transform.m_scale[i], transform.m_rotation[i]};
         write_transform(visual, iid, tf);
      }
   }
   core_internal void renderEntities(const ComponentVisual& visual, SharedData& shared, const Constants& constants, const GameResources& assets, GraphicsSystem& gfx, FontSystem& font)
   {
      auto count = getCount(visual);
      for( auto i = 0U; i < count; ++i )
      {
         gfx.renderMesh(visual.m_transform[i], visual.m_diffuse[i], shared.meshes[(u32)visual.m_meshId[i]]);
      }
   }
   core_internal std::vector<CollisionPair> findCollisions(ComponentCollision& collision, const ComponentTransform& transforms)
   {
      std::vector<CollisionPair> collidingEntities;
      auto count = getCount(collision);
      for( auto i = 0U; i < count; ++i )
      {
         for( auto j = i + 1; j < count; ++j )
         {
            if( shouldCollide(collision.m_collisionGroup[i], collision.m_collisionGroup[j],
               collision.m_selfTypeBits[i], collision.m_selfTypeBits[j],
               collision.m_targetTypeBits[i], collision.m_targetTypeBits[j]) )
            {
               auto shape1 = collision.m_shape[i];
               auto shape2 = collision.m_shape[j];
               auto eid1 = collision.m_entity[i];
               auto eid2 = collision.m_entity[j];
               auto iid1 = getDataId(transforms, eid1);
               auto iid2 = getDataId(transforms, eid2);
               CORE_ASSERT_DEBUG(iid1 != InvalidDataId, "Colliding entity [id=", eid1, "] has no transform");
               CORE_ASSERT_DEBUG(iid2 != InvalidDataId, "Colliding entity [id=", eid2, "] has no transform");
               setCollisionShapePosition(shape1, read_position(transforms, iid1));
               setCollisionShapePosition(shape2, read_position(transforms, iid2));
               setCollisionShapeScale(shape1, read_scale(transforms, iid1));
               setCollisionShapeScale(shape2, read_scale(transforms, iid2));
               auto collisionResult = areInCollision(shape1, shape2);
               if( collisionResult.isColliding )
               {
                  collision.m_previouslyColliding[i] = collision.m_colliding[i];
                  collision.m_previouslyColliding[j] = collision.m_colliding[j];
                  collision.m_colliding[i] = true;
                  collision.m_colliding[j] = true;
                  if( collision.m_sensor[i] || collision.m_sensor[j] )
                  {
                     collisionResult.displacement = {};
                  }
                  collidingEntities.push_back(CollisionPair{eid1, eid2, collisionResult.displacement});
               }
            }
         }
      }
      return collidingEntities;
   }
   core_internal void resolveCollisions(ComponentTransform& transform, std::vector<CollisionPair>& pairs)
   {
      auto count = getCount(transform);
      for( auto i = 0U; i < count; ++i )
      {
         Vec2 totalDisplacement{};
         for( auto p : pairs )
         {
            if( p.collider == transform.m_entity[i] || p.collidee == transform.m_entity[i] )
            {
               totalDisplacement += p.displacement;
            }
         }
         transform.m_position[i] += totalDisplacement;
      }
   }

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  GUI code
   //******************************************************************************************************************
   //******************************************************************************************************************
   template<int BUTTONS>
   core_internal void handle_mouseMove(GuiData<BUTTONS>& gui, Vec2 mousePosition)
   {
      gui.hoverButton = BUTTONS;
      for( auto i = 0; i < BUTTONS; ++i )
      {
         auto result = checkCollision(mousePosition, Rect{gui.button.position[i], gui.button.halfsize[i]});
         if( result.isColliding )
         {
            gui.hoverButton = i;
         }
      }
   }

   template<int BUTTONS>
   core_internal void handle_mouseDown(GuiData<BUTTONS>& gui)
   {
      gui.hotButton = gui.hoverButton;
   }

   template<int BUTTONS>
   core_internal u32 handle_mouseUp(GuiData<BUTTONS>& gui)
   {
      u32 activateButton = BUTTONS;
      if( gui.hotButton == gui.hoverButton )
      {
         activateButton = gui.hotButton;
      }
      gui.hotButton = BUTTONS;
      return activateButton;
   }

   template<int BUTTONS>
   core_internal u32 handle_guiInput(GuiData<BUTTONS>& gui, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents)
   {
      u32 activatedButton = BUTTONS;
      for( auto event : frameEvents )
      {
         switch( event.type )
         {
            case WinMsgType::MouseMove:
            {
               if( !event.mouse.move.relative )
               {
                  shared.mousePosition = orthoMousePosition(event.mouse.position, constants.windowWidth, constants.windowHeight);
               }
               else
               {
                  shared.mousePosition += {(f32)event.mouse.position.x, -(f32)event.mouse.position.y};
               }
               handle_mouseMove(gui, shared.mousePosition);
            } break;

            case WinMsgType::MouseButton:
            {
               if( event.mouse.button.id == Mouse::LeftButton )
               {
                  if( event.mouse.button.isDown )
                  {
                     handle_mouseDown(gui);
                  }
                  else
                  {
                     activatedButton = handle_mouseUp(gui);
                  }
               }
            } break;
         }
      }
      return activatedButton;
   }

   template<int BUTTONS>
   core_internal void render_guiData(GuiData<BUTTONS>& gui, SharedData& shared, const GameResources& assets, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.setOrthographicProjection();

      for( auto i = 0U; i < BUTTONS; ++i )
      {
         Transform buttonTransform{gui.button.position[i]};
         Color color = gui.button.idleColor[i];
         if( i == gui.hoverButton )
         {
            color = gui.button.hoverColor[i];
            if( i == gui.hotButton )
            {
               color = gui.button.hotColor[i];
            }
         }
         auto buttonFrameMesh = makeOutlineQuad({}, gui.button.halfsize[i], assets.mainVS, assets.mainPS);
         gfx.renderMesh(buttonTransform, color, buttonFrameMesh);

         auto buttonTextMesh = font.makeTextMesh(gui.button.caption[i].c_str(), shared.font, {1, 1}, TextJustification::Center, TextJustification::Middle);
         gfx.renderMesh(buttonTransform, {}, buttonTextMesh);
      }

      auto cursorMesh = makeSolidCircle({}, 3, 16, assets.mainVS, assets.mainPS);
      gfx.renderMesh({shared.mousePosition}, {}, cursorMesh);
   }


   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  main menu code
   //******************************************************************************************************************
   //******************************************************************************************************************
   core_internal bool init_mainMenu(MainMenuData& state, SharedData& shared, const Constants& constants, const GameResources& assets)
   {
      auto result = true;

      state.buttonFunctionToExecute = MainMenuData::COUNT;

      state.gui.hoverButton = state.COUNT;
      state.gui.hotButton = state.COUNT;

      Color defaultIdleColor{1, 1, 1};
      Color defaultHoverColor{1, 1, 0};
      Color defaultHotColor{1, 0, 0};

      auto i = 0;
      state.gui.button.position[i++] = {0, 100};
      state.gui.button.position[i++] = {0, -100};

      i = 0;
      state.gui.button.halfsize[i++] = {200, 50};
      state.gui.button.halfsize[i++] = {200, 50};

      i = 0;
      state.gui.button.idleColor[i++] = {0, 1, 0};
      state.gui.button.idleColor[i++] = {0, 1, 1};

      i = 0;
      state.gui.button.hoverColor[i++] = defaultHoverColor;
      state.gui.button.hoverColor[i++] = defaultHoverColor;

      i = 0;
      state.gui.button.hotColor[i++] = defaultHotColor;
      state.gui.button.hotColor[i++] = defaultHotColor;

      i = 0;
      state.gui.button.caption[i++] = "Start game";
      state.gui.button.caption[i++] = "Quit";

      return result;
   }

   core_internal State update_mainMenu(DeltaTime time, MainMenuData& state, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents,
                                       AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      auto nextState = State::MainMenu;
      auto activateButton = handle_guiInput(state.gui, shared, constants, frameEvents);

      if( activateButton != MainMenuData::COUNT )
      {
         switch( activateButton )
         {
            case MainMenuData::START_GAME:
            {
               nextState = State::GameplaySetup;
            } break;
            case MainMenuData::QUIT:
            {
               nextState = State::Quit;
            } break;
         }
      }

      return nextState;
   }

   core_internal void render_mainMenu(DeltaTime time, MainMenuData& state, SharedData& shared, const Constants& constants, const GameResources& assets,
                               GraphicsSystem& gfx, FontSystem& font)
   {
      render_guiData(state.gui, shared, assets, gfx, font);
   }

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  world code
   //******************************************************************************************************************
   //******************************************************************************************************************

   struct GameMessage
   {
      enum class Type
      {
         DirectionChange,
         AimChange,
         Fire,
         Ability,
      };
      Type type;
      Entity entity;
      union
      {
         Vec2 direction;
         Vec2 aim;
         bool shooting;
      };
      bool isAnalogue;
   };

   typedef std::vector<GameMessage> GameMessageVector;

   core_internal GameMessageVector translateInput(const EventVector_t& frameEvents, std::array<Entity, GameplayData::Numbers::PlayersCount>& players)
   {
      GameMessageVector result;
      for( const auto& e : frameEvents )
      {
         switch( e.type )
         {
            case WinMsgType::KeyboardKey:
            {
               GameMessage ge{};
               ge.entity = Entity{0}; //player 4
               ge.isAnalogue = false;
               if( e.keyboard.firstTimeDown )
               {
                  switch( e.keyboard.key.id )
                  {
                     case Keyboard::W:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {0, +1};
                        result.push_back(ge);
                     } break;
                     case Keyboard::S:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {0, -1};
                        result.push_back(ge);
                     } break;
                     case Keyboard::A:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {-1, 0};
                        result.push_back(ge);
                     } break;
                     case Keyboard::D:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {+1, 0};
                        result.push_back(ge);
                     } break;
                  }
               }
               else if( e.keyboard.key.isDown == false )
               {
                  switch( e.keyboard.key.id )
                  {
                     case Keyboard::W:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {0, -1};
                        result.push_back(ge);
                     } break;
                     case Keyboard::S:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {0, +1};
                        result.push_back(ge);
                     } break;
                     case Keyboard::A:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {+1, 0};
                        result.push_back(ge);
                     } break;
                     case Keyboard::D:
                     {
                        ge.type = GameMessage::Type::DirectionChange;
                        ge.direction = {-1, 0};
                        result.push_back(ge);
                     } break;
                  }
               }
            } break;

            case WinMsgType::GamepadAxis:
            {
               GameMessage ge{};
               ge.entity = Entity{e.gamepad.id};
               ge.isAnalogue = true;
               if( e.gamepad.axis.id == Gamepad::LeftStick )
               {
                  ge.type = GameMessage::Type::DirectionChange;
                  ge.direction = Vec2{e.gamepad.axis.x, e.gamepad.axis.y}*e.gamepad.axis.magnitude*e.gamepad.axis.normalized;
                  result.push_back(ge);
               }
               else if( e.gamepad.axis.id == Gamepad::RightStick )
               {
                  if( e.gamepad.axis.normalized >= 0.1 )
                  {
                     ge.type = GameMessage::Type::AimChange;
                     ge.aim = Vec2{e.gamepad.axis.x, e.gamepad.axis.y}*e.gamepad.axis.normalized;
                     result.push_back(ge);

                     GameMessage fire{};
                     fire.entity = Entity{e.gamepad.id};
                     ge.isAnalogue = true;
                     fire.shooting = true;
                     result.push_back(fire);
                  }
               }
            } break;

            case WinMsgType::MouseButton:
            {
               GameMessage ge{};
               ge.entity = Entity{0}; //player 4
               ge.isAnalogue = false;
               ge.type = GameMessage::Type::Fire;
               if( e.mouse.button.id == Mouse::LeftButton )
               {
                  ge.shooting = e.mouse.button.isDown;
                  result.push_back(ge);
               }
            } break;

            case WinMsgType::MouseMove:
            {
               GameMessage ge{};
               ge.entity = Entity{0}; //player 4
               ge.isAnalogue = false;
               ge.type = GameMessage::Type::AimChange;
               ge.aim = {(f32)e.mouse.position.x, (f32)e.mouse.position.y};
               result.push_back(ge);
            } break;
         }
      }
      std::sort(std::begin(result), std::end(result), [](const GameMessage& l, const GameMessage& r) { return l.type < r.type; });
      return result;
   }

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  world code
   //******************************************************************************************************************
   //******************************************************************************************************************

   core_internal void initWorld(World& world)
   {
#define REG_CALLBACK(what) world.destructionNotifier.registerCallback(makeDestructionCallback(what))
      REG_CALLBACK(world.deltaTime);
      REG_CALLBACK(world.transform);
      REG_CALLBACK(world.movement);
      REG_CALLBACK(world.visual);
      REG_CALLBACK(world.collision);
#undef REG_CALLBACK
   }

   core_internal void clearWorld(World& world)
   {
      world.destructionNotifier.clear();
      clearAll(world.deltaTime);
      clearAll(world.transform);
      clearAll(world.movement);
      clearAll(world.visual);
      clearAll(world.collision);
   }

   core_internal void simulateWorld(World& world, DeltaTime time, SharedData& shared, const Constants& constants,
                             const GameMessageVector& gameEvents)
   {
      advanceTimeForEntities(world.deltaTime, time.virt);
      for( const auto& ge : gameEvents )
      {
         switch( ge.type )
         {
            case GameMessage::Type::DirectionChange: // movement direction target (implies start/stop moving)
            {
               if( ge.isAnalogue )
               {
                  auto iid = getDataId(world.movement, ge.entity);
                  write_direction(world.movement, iid, ge.direction);
                  //session.targetDirection[ge.entity] = ge.direction;
               }
               else
               {
                  //session.targetDirection[ge.entity] += ge.direction;
               }
               //session.movement[ge.entity].acceleration = constants.playerAcceleration*vec2::length(vec2::normalize(session.targetDirection[ge.entity]));
            } break;
            //       - aim
            case GameMessage::Type::AimChange:
            {
               if( ge.isAnalogue )
               {
                  //session.aim[ge.entity].aim = ge.aim*constants.playerAimLength;
               }
               else
               {
                  //                   auto window = Vec2{constants.windowWidth, constants.windowHeight}*0.5f;
                  //                   gfx.setPerspectiveProjection();
                  //                   auto aim = session.aim[ge.entity].aim;
                  //                   aim = gfx.worldToScreen(camera, aim);
                  //                   aim += ge.aim;
                  //                   aim = gfx.screenToWorld(camera, aim);
                  //                   if( vec2::length2(aim) > pow2(constants.playerAimLength) )
                  //                   {
                  //                      session.aim[ge.entity].aim = vec2::setLength(aim, constants.playerAimLength);
                  //                   }
                  //                   else
                  //                   {
                  //                      session.aim[ge.entity].aim = aim;
                  //                   }
               }
            } break;
            //       - start/stop weapon fire
            case GameMessage::Type::Fire:
            {
               //state.shooting[ge.entity] = ge.shooting;
            } break;
            //       - ability use
            case GameMessage::Type::Ability:
            {
               //state.activateAbility[ge.entity] = true;
            } break;
         }
      }
   }

   //******************************************************************************************************************
   //******************************************************************************************************************
   //                                  gameplay setup code
   //******************************************************************************************************************
   //******************************************************************************************************************
   core_internal bool init_gameplaySetup(GameplayData& state, SharedData& shared, const Constants& constants, const GameResources& assets)
   {
      auto result = true;

      state.setupGui.hoverButton = GameplayData::GuiButton::COUNT;
      state.setupGui.hotButton = GameplayData::GuiButton::COUNT;

      Color defaultIdleColor{1, 1, 1};
      Color defaultHoverColor{1, 1, 0};
      Color defaultHotColor{1, 0, 0};

      auto i = 0;
      state.setupGui.button.position[i++] = {250 - constants.windowWidth / 2, constants.windowHeight / 2 - 150};

      i = 0;
      state.setupGui.button.halfsize[i++] = {200, 50};

      i = 0;
      state.setupGui.button.idleColor[i++] = defaultIdleColor;

      i = 0;
      state.setupGui.button.hoverColor[i++] = defaultHoverColor;

      i = 0;
      state.setupGui.button.hotColor[i++] = defaultHotColor;

      i = 0;
      state.setupGui.button.caption[i++] = "Back to main menu";

      //circle entity for detecting setup state
      auto eid = state.startArea = state.world.entityManager.create();

      auto iid = createData(state.world.transform, eid);
      write_scale(state.world.transform, iid, {10, 10});

      iid = createData(state.world.visual, eid);
      write_transform(state.world.visual, iid, {{}, {10, 10}});
      write_diffuse(state.world.visual, iid, {1, 1, 1, 0.5f});
      write_meshId(state.world.visual, iid, MeshId::StartArea);

      iid = createData(state.world.collision, eid);
      CollisionShape shape;
      shape.type = CollisionShape::Type::CircleShape;
      shape.circle.center = {0, 0};
      shape.circle.radius = 5;
      write_shape(state.world.collision, iid, shape);
      write_sensor(state.world.collision, iid, true);
      //other data


      eid = state.players[0] = state.world.entityManager.create();

      iid = createData(state.world.deltaTime, eid);
      iid = createData(state.world.transform, eid);
      iid = createData(state.world.movement, eid);
      iid = createData(state.world.visual, eid);
      write_meshId(state.world.visual, iid, MeshId::KeyboardPlayer);
      iid = createData(state.world.collision, eid);
      shape.circle.radius = 0.5f;
      write_shape(state.world.collision, iid, shape);

      return result;
   }

   core_internal State update_gameplaySetup(DeltaTime time, GameplayData& state, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents,
                                     AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      auto nextState = State::GameplaySetup;
      u32 activatedButton = handle_guiInput(state.setupGui, shared, constants, frameEvents);

      if( activatedButton == GameplayData::GuiButton::BACK )
      {
         nextState = State::MainMenu;
      }

      //phase where input events are translated to game events
      std::vector<GameMessage> gameMsgs;

      advanceTimeForEntities(state.world.deltaTime, time.virt);
      //handle input
      //handle AI
      //handle physics
      turnEntities(state.world.movement, state.world.deltaTime);
      moveEntities(state.world.movement, state.world.deltaTime);
      syncPositionAfterMovement(state.world.movement, state.world.transform);
      auto collisions = findCollisions(state.world.collision, state.world.transform);
      resolveCollisions(state.world.transform, collisions);
      //handle collision responses
      //check if one entity is the start circle, and the other a player
      for( auto p : collisions )
      {
         if( p.collider == state.startArea && std::any_of(std::begin(state.players), std::end(state.players), [p](Entity e) { return p.collidee == e; }) )
         {

         }
         else if( p.collidee == state.startArea && std::any_of(std::begin(state.players), std::end(state.players), [p](Entity e) { return p.collider == e; }) )
         {

         }
      }

      return nextState;
   }

   core_internal void render_gameplaySetup(DeltaTime time, GameplayData& state, SharedData& shared, const Constants& constants, const GameResources& assets,
                                    GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.setPerspectiveProjection();

      //prepare the renderable data only before actually rendering
      syncPositionForRendering(state.world.transform, state.world.visual);
      renderEntities(state.world.visual, shared, constants, assets, gfx, font);

      render_guiData(state.setupGui, shared, assets, gfx, font);
   }

   core_internal void loadMeshBundle(f32 left, f32 top, f32 right, f32 bottom, u32 columns, u32 rows,
                              TextureManager& textures, HTexture texture, HVertexShader vertex, HPixelShader pixel,
                              std::vector<Mesh>& outMeshes)
   {
      f32 width = right - left;
      f32 height = bottom - top;
      f32 w = width / columns;
      f32 h = height / rows;
      f32 hw = w * 0.5f;
      f32 hh = h * 0.5f;
      f32 smallerDimension = hw > hh ? hh : hw;
      hw /= smallerDimension;
      hh /= smallerDimension;
      auto txtr = textures.getData(texture);
      f32 tw = (f32)txtr.width;
      f32 th = (f32)txtr.height;
      for( u32 y = 0; y < rows; ++y )
      {
         for( u32 x = 0; x < columns; ++x )
         {
            auto mesh = makeTexturedQuad({}, {hw, hh}, texture, {(left + w*x) / tw, (top + h*y) / th}, {(left + w*(x + 1)) / tw, (top + h*(y + 1)) / th}, vertex, pixel);
            outMeshes.push_back(mesh);
         }
      }
   }

   core_internal Mesh makeMesh(f32 x, f32 y, f32 w, f32 h, TextureManager& textures, HTexture texture, HVertexShader vertex, HPixelShader pixel)
   {
      auto txtr = textures.getData(texture);
      f32 tw = (f32)txtr.width;
      f32 th = (f32)txtr.height;
      f32 hw = w * 0.5f;
      f32 hh = h * 0.5f;
      f32 smallerDimension = hw > hh ? hh : hw;
      hw /= smallerDimension;
      hh /= smallerDimension;
      auto result = makeTexturedQuad({}, {hw, hh}, texture, {x / tw, y / th}, {(x + w) / tw, (y + h) / th}, vertex, pixel);
      return result;
   }

   core_internal std::vector<Mesh> loadMeshes(GameData& game, TextureManager& textures)
   {
      std::vector<Mesh> result{};
      //players
      loadMeshBundle(1795, 420, 3085, 673, 5, 1, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS, result);

      return result;
   }

   /*
      struct PlayerData
      {
      f32 timeFactor;
      Vec2 startingPosition;
      Color color;
      MeshId startingMeshId;
      Vec2 startingAim;
      CollisionShape collisionShape;
      bool sensor;
      };

      struct BulletData
      {
      f32 timeFactor;
      Vec2 startingPosition;
      Vec2 direction;
      f32 speed;
      };
      */

   enum class CollisionCenteringType
   {
      Clear,
      Center,
   };

   /*


      core_internal void toggleCollisionState(SessionState& session)
      {
      for( u32 e = 0; e < session.entityCount; ++e )
      {
      session.collision[e].previouslyInCollision = session.collision[e].currentlyInCollision;
      session.collision[e].currentlyInCollision = false;
      }
      }

      core_internal std::vector<CollisionPair> findCollisions(SessionState& session)
      {
      setCollisionCenter(session, CollisionCenteringType::Center);
      toggleCollisionState(session);

      std::vector<CollisionPair> result;
      for( u32 e = 0; e < session.entityCount; ++e )
      {
      for( u32 r = e + 1; r < session.entityCount; ++r )
      {
      bool testCollision = shouldCollide(session.collision[e], session.collision[r]);

      if( testCollision )
      {
      auto testResult = areInCollision(session.collision[e].shape, session.collision[r].shape);
      if( testResult.isColliding )
      {
      result.push_back({e, r, testResult.displacement});
      if( session.collision[e].sensor || session.collision[r].sensor )
      {
      result.back().displacement = {};
      }
      }
      session.collision[e].currentlyInCollision = session.collision[e].currentlyInCollision || testResult.isColliding;
      session.collision[r].currentlyInCollision = session.collision[r].currentlyInCollision || testResult.isColliding;
      }
      }
      }

      setCollisionCenter(session, CollisionCenteringType::Clear);
      return result;
      }*/

   //*****************************************************************
   //          SESSION TRANSLATE INPUT
   //*****************************************************************

   core_internal State update_gameplaySession(DeltaTime time, Constants& constants, GameplayData& state, const EventVector_t& frameEvents,
                                              AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx, Camera& camera)
   {
      //    preamble
      //       - update all timers
      //    player input based updates (for specific player):
      /*
      translate window events into game events based on what the allowed controls are
      allowed actions the player should be able to do:

      keyboard: pressing WSAD changes the target direction of the player from (0,0) by adding a 1 to the axis, releasing WSAD subtracts the 1 off the axis
      gamepad: moving the left stick away from it's deadzone generates a normalized target direction vector which is mapped directly
      whenever the target direction is not (0,0) the player is assumed to be moving
      the aim cursor is always relative to the player
      mouse: moving the mouse should move the aim cursor relative to the player, not the screen, which generates a normalized aim direction
      gamepad: moving the right stick generates a normalized aim direction
      pressing a specific gamepad button or keyboard/mouse key activates the class ability of the player
      any extra optional actions could be considered
      */
      //the translator is a system with registered mappings from WindowEvent to GameEvent
      //done in the init function when we know how many players we have and which input device they are using
      //    AI updates:
      //       - decide if should be moving
      //       - decide location to move to, calculate movement direction target
      //       - update aim based on movement direction
      //    simulation updates:
      //       - update movement direction based on target direction
      //       - propose movement in current direction
      //       - players, monsters and rockets work the same
      //       - bullets could be made to work the same with some changes to their data
      //       - blasts are a bit more tricky, might have to change how they work entirely

      //       for( u32 e = 0; e < session.entityCount; ++e )
      //       {
      //          auto acceleration = session.movement[e].direction * session.movement[e].acceleration;
      //          acceleration += -session.movement[e].velocity * 10.0f;
      //          session.movement[e].position = acceleration*0.5f*pow2(session.deltaTime[e].deltaTime) + session.movement[e].velocity * session.deltaTime[e].deltaTime + session.position[e].position;
      //          session.movement[e].velocity = acceleration*session.deltaTime[e].deltaTime + session.movement[e].velocity;
      //       }

      //       - find collisions for new position
      //      auto collisionPairs = findCollisions(session);
      //       - [resolve collisions via displacement vector]->optional depending on performance
      //       - accept new position with displacement vector added
      //       for( u32 e = 0; e < session.entityCount; ++e )
      //       {
      //          if( session.position[e].position != session.movement[e].position )
      //          {
      //             session.position[e].position = session.movement[e].position;
      //          }
      //       }
      //       setCollisionCenter(session, CollisionCenteringType::Center);
      //       for( auto& pair : collisionPairs )
      //       {
      //          session.position[pair.collider].position += pair.displacement;
      //       }
      //       setCollisionCenter(session, CollisionCenteringType::Clear);
      /*for( u32 e = 0; e < session.entityCount; ++e )
      {
      if( isStartCollision(session.collision[e]) )
      {
      session.render[e].color = {1, 0, 0};
      }
      else if( isEndCollision(session.collision[e]) )
      {
      session.render[e].color = {0, 1, 0};
      }
      }*/
      //       - fix camera to new position and zoom level
      //    gameplay update:
      //       - collision response
      //       - damage calculation
      //       - bullets hurt monsters
      //       - blasts hurt monsters
      //       - rockets hurt monsters (and spawn blasts)
      //       - monsters hurt players
      //       - [players hurt monsters]->if they have a perk for it
      //       - pickup collection
      //       - bonuses
      //       - weapons
      //       - abilities
      //       - each ability has unique logic for handling collisions with all other entities
      //       - level up + perk mode (players only? maybe make the monster that makes a player kill bigger, turn him into a boss or something...)
      //       - death (monsters AND players)
      //       - active bonuses
      //       - acquired perks
      //       - fire weapons
      //       - new spawns (projectiles, blasts, monsters, pickups, blood splatter)
      //       - difficulty increase
      //    visual updates:
      //       - change orientation based on aim direction
      //       for( u32 e = 0; e < session.entityCount; ++e )
      //       {
      //          session.render[e].rotationRadians = radians(session.aim[e].aim);
      //       }
      //       - animation
      //       - transparency
      return State::GameplaySession;
   }

   core_internal void render_gameplaySession(DeltaTime time, GameplayData& session, GameResources assets, GraphicsSystem& gfx, FontSystem& font, /*temp*/ std::vector<Mesh>& meshes)
   {
      gfx.setPerspectiveProjection();
      gfx.setTransparency(true);

      //       for( u32 e = 0; e < session.entityCount; ++e )
      //       {
      //          gfx.renderMesh({session.position[e].position, {1, 1}, session.render[e].rotationRadians}, {}, meshes[session.render[e].mesh]);
      //          auto aimMesh = makeSolidCircle({}, 0.2f, 8, assets.mainVS, assets.mainPS);
      //          gfx.renderMesh({session.position[e].position + session.aim[e].aim}, session.render[e].color, aimMesh);
      //          Mesh collisionMesh{};
      //          switch( session.collision[e].shape.type )
      //          {
      //             case CollisionShape::CircleShape:
      //             {
      //                collisionMesh = makeOutlineCircle(session.collision[e].shape.circle, 16, assets.mainVS, assets.mainPS);
      //             } break;
      //             case CollisionShape::RectShape:
      //             {
      //                collisionMesh = makeOutlineQuad(session.collision[e].shape.rect, assets.mainVS, assets.mainPS);
      //             } break;
      //          }
      //          gfx.renderMesh({session.position[e].position}, session.render[e].color, collisionMesh);
      //          auto moveDir = makeLine(Vec2{}, session.movement[e].direction, assets.mainVS, assets.mainPS);
      //          gfx.renderMesh(session.position[e].position, {}, moveDir);
      //       }
   }



   core_internal bool transition_state(GameData& game)
   {
      auto result = true;
      if( game.nextState != game.currentState )
      {
         // #todo cleanup call for the current state before switching
         game.currentState = game.nextState;
         switch( game.currentState )
         {
            case State::MainMenu:
            {
               result = init_mainMenu(game.mainMenu, game.sharedData, game.constants, game.assets);
            } break;

            case State::GameplaySetup:
            {
               result = init_gameplaySetup(game.gameplay, game.sharedData, game.constants, game.assets);
            } break;

            case State::GameplaySession:
            {
               //result = init_session(game, game.session);
            } break;
         }
      }
      return result;
   }

   core_internal bool init_game(GameData& game, AudioSystem& audio, GraphicsSystem& gfx, LuaStack lua)
   {
      bool result = true;
      game.assets = loadGameResources(audio.sounds, gfx.pixelShaders, gfx.vertexShaders, gfx.textures);

      result = checkGameResourcesLoaded(game.assets);

      if( result )
      {
         game.sharedData.meshes.reserve((u32)MeshId::MeshCount);
         //game.sharedData.meshes = loadMeshes(game, gfx.textures);
         //game.sharedData.meshes.push_back({});
         //game.sharedData.meshes[LineMesh] = makeLine({0, 0}, {-1, 0}, game.assets.mainVS, game.assets.mainPS);
         game.sharedData.meshes.emplace_back(makeSolidCircle({}, 1, 32, game.assets.mainVS, game.assets.mainPS));
         game.sharedData.meshes.emplace_back(makeSolidCircle({}, 1, 32, game.assets.mainVS, game.assets.mainPS));

         game.sharedData.font = loadFont(lua, CORE_RESOURCE("Defs/font.font"), game.assets.font, game.assets.mainVS, game.assets.mainPS);

         game.sharedData.camera.setPosition({0, 0, -50});
         game.sharedData.mousePosition = {0, 0};

         game.constants.playerAcceleration = 60;
         game.constants.playerAimLength = 4;
         game.sharedData.showCursor = Cursor_Hide;
         game.sharedData.lockCursor = Cursor_Lock;
         game.sharedData.relativeCursor = Cursor_Absolute;

         game.currentState = State::Startup;
         game.nextState = State::MainMenu;
         result = transition_state(game);
      }

      return true;
   }

   core_internal bool update_game(DeltaTime time, GameData& game, const EventVector_t& frameEvents, AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      bool stillRunning = true;
      enum
      {
         STATE,
         ACTION,
      };

      auto contains = [&frameEvents](u32 k, u32 s) -> bool
      {
         for( auto e : frameEvents )
         {
            if( e.type == WinMsgType::KeyboardKey && e.keyboard.key.id == k && e.keyboard.key.isDown && (s == STATE || e.keyboard.firstTimeDown) )
            {
               return true;
            }
         }
         return false;
      };

      switch( game.currentState )
      {
         case State::MainMenu:
         {
            game.nextState = update_mainMenu(time, game.mainMenu, game.sharedData, game.constants, frameEvents, audio, lua, gfx);

            if( contains(Keyboard::Space, ACTION) )
            {
               game.nextState = State::GameplaySetup;
            }
         } break;
         case State::GameplaySetup:
         {
            game.nextState = update_gameplaySetup(time, game.gameplay, game.sharedData, game.constants, frameEvents, audio, lua, gfx);

            if( contains(Keyboard::Space, ACTION) )
            {
               game.nextState = State::GameplaySession;
            }
         } break;
         case State::GameplaySession:
         {
            //game.nextState = update_gameplaySession(time, game.constants, game.session, frameEvents, audio, lua, gfx, game.sharedData.camera);

            if( contains(Keyboard::Space, ACTION) )
            {
               game.nextState = State::Score;
            }
         } break;
         case State::Score:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               game.nextState = State::MainMenu;
            }
         } break;
         case State::Quit:
         {
            stillRunning = false;
         } break;
      }

      stillRunning = stillRunning && transition_state(game);

      return stillRunning;
   }

   core_internal void render_game(DeltaTime time, GameData& game, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.applyCamera(game.sharedData.camera);
      /*
      systems.gfx->setPerspectiveProjection();
      systems.gfx->setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->setTransparency(true);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
      */

      std::string caption;
      switch( game.currentState )
      {
         case State::MainMenu:
         {
            caption = "Main menu";

            render_mainMenu(time, game.mainMenu, game.sharedData, game.constants, game.assets, gfx, font);
         } break;
         case State::GameplaySetup:
         {
            caption = "Gameplay setup";

            render_gameplaySetup(time, game.gameplay, game.sharedData, game.constants, game.assets, gfx, font);
         } break;
         case State::GameplaySession:
         {
            caption = "Gameplay session";

            //render_gameplaySession(time, game.session, game.assets, gfx, font, game.sharedData.meshes);
         } break;
         case State::Score:
         {
            caption = "Score screen";
         } break;
         case State::Quit:
         {
            caption = "Quitting...";
         } break;
         case State::Shutdown:
         {
            caption = "Shutdown";
         } break;
         case State::Startup:
         {
            caption = "Startup";
         } break;
      }

      gfx.setOrthographicProjection();
      gfx.setTransparency(true);
      caption += (time.virt.micros == 0 ? " (paused)" : " (running)");
      auto mesh = font.makeTextMesh(caption.c_str(), game.sharedData.font, {1, 1}, Left, Top);
      gfx.renderMesh(Vec2{-game.constants.windowWidth*0.5f, game.constants.windowHeight*0.5f}, {}, mesh);
   }

}
