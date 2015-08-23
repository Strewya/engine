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
#include "util/collision_checks.h"
/******* end headers *******/

namespace core
{
   inline float pow2(float v)
   {
      auto result = v*v;
      return result;
   }

   inline float radians(Vec2 v)
   {
      auto result = std::atan2(v.y, v.x);
      return result;
   }

   template<typename T>
   static CollisionResult areInCollision(T one, CollisionShape two)
   {
      CollisionResult result{};

      switch( two.type )
      {
         case CollisionShape::PointShape:
         {
            result = checkCollision(one, two.point);
         } break;
         case CollisionShape::CircleShape:
         {
            result = checkCollision(one, two.circle);
         } break;
         case CollisionShape::RectShape:
         {
            result = checkCollision(one, two.rect);
         } break;
      }

      return result;
   }

   static CollisionResult areInCollision(CollisionShape one, CollisionShape two)
   {
      CollisionResult result{};

      switch( one.type )
      {
         case CollisionShape::PointShape:
         {
            result = areInCollision(one.point, two);
         } break;
         case CollisionShape::CircleShape:
         {
            result = areInCollision(one.circle, two);
         } break;
         case CollisionShape::RectShape:
         {
            result = areInCollision(one.rect, two);
         } break;
      }

      return result;
   }

   template<typename T>
   static bool fullyContained(T one, CollisionShape two)
   {
      auto result = false;
      switch( two.type )
      {
         case CollisionShape::PointShape:
         {
            result = isFullyWithin(one, two.point);
         } break;
         case CollisionShape::CircleShape:
         {
            result = isFullyWithin(one, two.circle);
         } break;
         case CollisionShape::RectShape:
         {
            result = isFullyWithin(one, two.rect);
         } break;
      }

      return result;
   }

   static bool fullyContained(CollisionShape one, CollisionShape two)
   {
      auto result = false;
      switch( one.type )
      {
         case CollisionShape::PointShape:
         {
            result = fullyContained(one.point, two);
         } break;
         case CollisionShape::CircleShape:
         {
            result = fullyContained(one.circle, two);
         } break;
         case CollisionShape::RectShape:
         {
            result = fullyContained(one.rect, two);
         } break;
      }
      return result;
   }





   static const DataId InvalidDataId{(std::numeric_limits<uint32_t>::max())};

   static bool operator==(DataId l, DataId r)
   {
      auto result = l.id == r.id;
      return result;
   }
   static bool operator!=(DataId l, DataId r)
   {
      auto result = !(l == r);
      return result;
   }


   namespace component_impl
   {
      template<typename DATA>
      static void requestSlot(DATA& data, DataId i, typename DATA::value_type defaultValue = {})
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

      static DataId requestSlot(ComponentBase& cmp, Entity e)
      {
         DataId id{cmp.m_mapping.size()};
         cmp.m_mapping.insert({e, id});
         requestSlot(cmp.m_entity, id, e);
         return id;
      }

      template<typename DATA>
      void releaseSlot(DATA& data, std::pair<DataId, DataId> ids)
      {
         if( ids.first != InvalidDataId && ids.second != InvalidDataId )
         {
            std::swap(data[ids.first.id], data[ids.second.id]);
         }
      }

      std::pair<DataId, DataId> releaseSlot(ComponentBase& cmp, Entity e)
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
   static DataId getDataId(ComponentBase& cache, Entity e)
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
   static bool containsData(ComponentBase& cache, Entity e)
   {
      auto id = getDataId(cache, e);
      auto result = (id != InvalidDataId);
      return result;
   }
   //*************************************************
   //********************* CLEAR *********************
   //*************************************************
   static void clearAll(ComponentBase& cache)
   {
      cache.m_mapping.clear();
   }
   //*****************************************************
   //********************* GET COUNT *********************
   //*****************************************************
   static uint32_t getCount(ComponentBase& cache)
   {
      auto result = cache.m_mapping.size();
      return result;
   }
   //*******************************************************
   //********************* CREATE DATA *********************
   //*******************************************************
   static DataId createData(ComponentDeltaTime& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_deltaMicros, iid);
      component_impl::requestSlot(cache.m_deltaTime, iid);
      component_impl::requestSlot(cache.m_timeFactor, iid, 1.0f);
      return iid;
   }
   static DataId createData(ComponentMovement& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_acceleration, iid);
      component_impl::requestSlot(cache.m_direction, iid);
      component_impl::requestSlot(cache.m_velocity, iid);
      component_impl::requestSlot(cache.m_position, iid);
      return iid;
   }
   static DataId createData(ComponentPosition& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_position, iid);
      return iid;
   }
   static DataId createData(ComponentVisual& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_transform, iid);
      component_impl::requestSlot(cache.m_color, iid);
      component_impl::requestSlot(cache.m_meshId, iid);
      return iid;
   }
   static DataId createData(ComponentCollision& cache, Entity e)
   {
      auto iid = component_impl::requestSlot(cache, e);
      component_impl::requestSlot(cache.m_shape, iid);
      component_impl::requestSlot(cache.m_collisionGroup, iid);
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
   static void releaseData(ComponentDeltaTime& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_deltaMicros, iid);
      component_impl::releaseSlot(cache.m_deltaTime, iid);
      component_impl::releaseSlot(cache.m_timeFactor, iid);
   }
   static void releaseData(ComponentMovement& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_acceleration, iid);
      component_impl::releaseSlot(cache.m_direction, iid);
      component_impl::releaseSlot(cache.m_velocity, iid);
      component_impl::releaseSlot(cache.m_position, iid);
   }
   static void releaseData(ComponentPosition& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_position, iid);
   }
   static void releaseData(ComponentVisual& cache, Entity e)
   {
      auto iid = component_impl::releaseSlot(cache, e);
      component_impl::releaseSlot(cache.m_transform, iid);
      component_impl::releaseSlot(cache.m_color, iid);
      component_impl::releaseSlot(cache.m_meshId, iid);
   }
   static void releaseData(ComponentCollision& cache, Entity e)
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
   static DestructionCallback makeDestructionCallback(COMPONENT& cmp)
   {
      return std::bind<void(COMPONENT&, Entity)>(releaseData, std::ref(cmp), std::placeholders::_1);
   }
   //************************************************
   //********************* READ *********************
   //************************************************
#define READ_FN(storage, field) \
static auto read_##field(storage& cache, DataId id) -> std::remove_reference<decltype(cache.m_##field[id.id])>::type { return cache.m_##field[id.id]; }
   //************************************************
   READ_FN(ComponentDeltaTime, timeFactor);
   READ_FN(ComponentDeltaTime, deltaTime);
   READ_FN(ComponentDeltaTime, deltaMicros);
   READ_FN(ComponentMovement, position);
   READ_FN(ComponentPosition, position);
   READ_FN(ComponentVisual, transform);
#undef READ_FN
   //*************************************************
   //********************* WRITE *********************
   //************************************************
#define WRITE_FN(storage, field) \
static void write_##field(storage& cache, DataId id, std::remove_reference<decltype(cache.m_##field[id.id])>::type value) { cache.m_##field[id.id] = value; }
   //*************************************************
   WRITE_FN(ComponentDeltaTime, timeFactor);
   WRITE_FN(ComponentPosition, position);
   WRITE_FN(ComponentMovement, position);
   WRITE_FN(ComponentMovement, acceleration);
   WRITE_FN(ComponentMovement, direction);
   WRITE_FN(ComponentVisual, transform);
   WRITE_FN(ComponentVisual, color);
   WRITE_FN(ComponentVisual, meshId);
#undef WRITE_FN
   //*************************************************************
   //********************* SYSTEM OPERATIONS *********************
   //*************************************************************
   static void advanceTimeForEntities(ComponentDeltaTime& cache, Time dt)
   {
      auto count = getCount(cache);
      for( auto i = 0U; i < count; ++i )
      {
         auto factor = cache.m_timeFactor[i];
         cache.m_deltaMicros[i] = static_cast<uint32_t>(dt.micros*factor);
         cache.m_deltaTime[i] = dt.seconds*factor;
      }
   }
   static void moveEntities(ComponentMovement& movement, ComponentDeltaTime& deltaTimes)
   {
      auto count = movement.m_mapping.size();
      for( auto i = 0U; i < count; ++i )
      {
         auto e = movement.m_entity[i];
         auto iid = getDataId(deltaTimes, e);
         auto dt = read_deltaTime(deltaTimes, iid);
         auto acceleration = movement.m_direction[i] * movement.m_acceleration[i];
         acceleration += -movement.m_velocity[i] * 10.0f;
         movement.m_position[i] = acceleration*0.5f*pow2(dt) + movement.m_velocity[i] * dt + movement.m_position[i];
         movement.m_velocity[i] = acceleration*dt + movement.m_velocity[i];
      }
   }
   static void syncPositionAfterMovement(ComponentMovement& movement, ComponentPosition& position)
   {
      auto count = getCount(movement);
      for( auto i = 0U; i < count; ++i )
      {
         auto entity = movement.m_entity[i];
         auto iid = getDataId(position, entity);
         write_position(position, iid, movement.m_position[i]);
      }
   }
   static void syncPositionForRendering(ComponentPosition& position, ComponentVisual& visual)
   {
      auto count = getCount(position);
      for( auto i = 0U; i < count; ++i )
      {
         auto entity = position.m_entity[i];
         auto iid = getDataId(visual, entity);
         auto tf = read_transform(visual, iid);
         tf.position = position.m_position[i];
         write_transform(visual, iid, tf);
      }
   }
   static void renderEntities(ComponentVisual& visual, ComponentPosition& position,
                              SharedData& shared, const Constants& constants, const GameResources& assets,
                              GraphicsSystem& gfx, FontSystem& font)
   {
      auto count = getCount(visual);
      for( auto i = 0U; i < count; ++i )
      {
         gfx.renderMesh(visual.m_transform[i], visual.m_color[i], shared.meshes[(uint32_t)visual.m_meshId[i]]);
      }
   }





   static bool init_mainMenu(MainMenuData& state, SharedData& shared, const Constants& constants, const GameResources& assets)
   {
      auto result = true;
      /*
            shared.lockCursor = true;
            shared.relativeCursor = true;
            shared.showCursor = false;
            */
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

   static Vec2 orthoMousePosition(MousePosition mouse, float windowWidth, float windowHeight)
   {
      Vec2 result;

      result.x = mouse.x - windowWidth / 2;
      result.y = -mouse.y + windowHeight / 2;

      return result;
   }

   template<int BUTTONS>
   static void handle_mouseMove(GuiData<BUTTONS>& gui, Vec2 mousePosition)
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
   static void handle_mouseDown(GuiData<BUTTONS>& gui)
   {
      gui.hotButton = gui.hoverButton;
   }

   template<int BUTTONS>
   static uint32_t handle_mouseUp(GuiData<BUTTONS>& gui)
   {
      uint32_t activateButton = BUTTONS;
      if( gui.hotButton == gui.hoverButton )
      {
         activateButton = gui.hotButton;
      }
      gui.hotButton = BUTTONS;
      return activateButton;
   }

   template<int BUTTONS>
   static uint32_t handle_guiInput(GuiData<BUTTONS>& gui, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents)
   {
      uint32_t activatedButton = BUTTONS;
      for( auto event : frameEvents )
      {
         switch( event.type )
         {
            case WE_MOUSEMOVE:
            {
               if( !event.mouse.move.relative )
               {
                  shared.mousePosition = orthoMousePosition(event.mouse.position, constants.windowWidth, constants.windowHeight);
               }
               else
               {
                  shared.mousePosition += {(float)event.mouse.position.x, -(float)event.mouse.position.y};
               }
               handle_mouseMove(gui, shared.mousePosition);
            } break;

            case WE_MOUSEBUTTON:
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

   static State update_mainMenu(DeltaTime time, MainMenuData& state, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents,
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

   template<int BUTTONS>
   static void render_guiData(GuiData<BUTTONS>& gui, SharedData& shared, const GameResources& assets, GraphicsSystem& gfx, FontSystem& font)
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

   static void render_mainMenu(DeltaTime time, MainMenuData& state, SharedData& shared, const Constants& constants, const GameResources& assets,
                               GraphicsSystem& gfx, FontSystem& font)
   {
      render_guiData(state.gui, shared, assets, gfx, font);
   }

   enum class MeshId
   {
      StartArea,
      MeshCount
   };

   static void clearWorld(World& world)
   {
      world.destructionNotifier.clear();
      clearAll(world.deltaTime);
      clearAll(world.position);
      clearAll(world.movement);
      clearAll(world.visual);
   }

   static void initWorld(World& world)
   {
      world.destructionNotifier.registerCallback(makeDestructionCallback(world.deltaTime));
      world.destructionNotifier.registerCallback(makeDestructionCallback(world.position));
      world.destructionNotifier.registerCallback(makeDestructionCallback(world.movement));
      world.destructionNotifier.registerCallback(makeDestructionCallback(world.visual));
   }

   static bool init_gameplaySetup(GameplayData& state, SharedData& shared, const Constants& constants, const GameResources& assets)
   {
      auto result = true;
      /*
            shared.lockCursor = false;
            shared.relativeCursor = true;
            shared.showCursor = true;
            */
      state.setupGui.hoverButton = state.COUNT;
      state.setupGui.hotButton = state.COUNT;

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

      //circle entity for detecting game start 
      Entity eid = state.world.entityManager.create();
      state.entities.emplace_back(eid);
      auto iid = createData(state.world.deltaTime, eid);
      iid = createData(state.world.position, eid);
      iid = createData(state.world.visual, eid);
      write_transform(state.world.visual, iid, {{}, {10, 10}});
      write_color(state.world.visual, iid, {1, 1, 1, 0.5f});
      write_meshId(state.world.visual, iid, MeshId::StartArea);
      iid = createData(state.world.collision, eid);
      
      return result;
   }

   static State update_gameplaySetup(DeltaTime time, GameplayData& state, SharedData& shared, const Constants& constants, const EventVector_t& frameEvents,
                                     AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      //do the update
      auto nextState = State::GameplaySetup;
      uint32_t activatedButton = handle_guiInput(state.setupGui, shared, constants, frameEvents);

      if( activatedButton == GameplayData::BACK )
      {
         nextState = State::MainMenu;
      }
      advanceTimeForEntities(state.world.deltaTime, time.virt);
      moveEntities(state.world.movement, state.world.deltaTime);
      syncPositionAfterMovement(state.world.movement, state.world.position);

      return nextState;
   }

   static void render_gameplaySetup(DeltaTime time, GameplayData& state, SharedData& shared, const Constants& constants, const GameResources& assets,
                                    GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.setPerspectiveProjection();

      //prepare the renderable data only before actually rendering
      syncPositionForRendering(state.world.position, state.world.visual);
      renderEntities(state.world.visual, state.world.position, shared, constants, assets, gfx, font);

      render_guiData(state.setupGui, shared, assets, gfx, font);
   }

   static void loadMeshBundle(float left, float top, float right, float bottom, uint32_t columns, uint32_t rows,
                              TextureManager& textures, HTexture texture, HVertexShader vertex, HPixelShader pixel,
                              std::vector<Mesh>& outMeshes)
   {
      float width = right - left;
      float height = bottom - top;
      float w = width / columns;
      float h = height / rows;
      float hw = w * 0.5f;
      float hh = h * 0.5f;
      float smallerDimension = hw > hh ? hh : hw;
      hw /= smallerDimension;
      hh /= smallerDimension;
      auto txtr = textures.getData(texture);
      float tw = (float)txtr.width;
      float th = (float)txtr.height;
      for( uint32_t y = 0; y < rows; ++y )
      {
         for( uint32_t x = 0; x < columns; ++x )
         {
            auto mesh = makeTexturedQuad({}, {hw, hh}, texture, {(left + w*x) / tw, (top + h*y) / th}, {(left + w*(x + 1)) / tw, (top + h*(y + 1)) / th}, vertex, pixel);
            outMeshes.push_back(mesh);
         }
      }
   }

   static Mesh makeMesh(float x, float y, float w, float h, TextureManager& textures, HTexture texture, HVertexShader vertex, HPixelShader pixel)
   {
      auto txtr = textures.getData(texture);
      float tw = (float)txtr.width;
      float th = (float)txtr.height;
      float hw = w * 0.5f;
      float hh = h * 0.5f;
      float smallerDimension = hw > hh ? hh : hw;
      hw /= smallerDimension;
      hh /= smallerDimension;
      auto result = makeTexturedQuad({}, {hw, hh}, texture, {x / tw, y / th}, {(x + w) / tw, (y + h) / th}, vertex, pixel);
      return result;
   }

   static std::vector<Mesh> loadMeshes(GameData& game, TextureManager& textures)
   {
      std::vector<Mesh> result{};
      //players
      loadMeshBundle(1795, 420, 3085, 673, 5, 1, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS, result);

      return result;
   }

   /*
      struct PlayerData
      {
      float timeFactor;
      Vec2 startingPosition;
      Color color;
      MeshId startingMeshId;
      Vec2 startingAim;
      CollisionShape collisionShape;
      bool sensor;
      };

      struct BulletData
      {
      float timeFactor;
      Vec2 startingPosition;
      Vec2 direction;
      float speed;
      };
      */




   static bool shouldCollide(const CollisionData& d1, const CollisionData& d2)
   {
      if( d1.collisionGroup == d2.collisionGroup && d1.collisionGroup != 0 )
      {
         return d1.collisionGroup > 0;
      }

      bool masks = (d1.selfTypeBits & d2.targetTypeBits) != 0 &&
         (d2.selfTypeBits & d1.targetTypeBits) != 0;
      return masks;
   }

   static bool isStartCollision(const CollisionData& d)
   {
      auto result = d.previouslyInCollision == false && d.currentlyInCollision == true;
      return result;
   }

   static bool isEndCollision(const CollisionData& d)
   {
      auto result = d.previouslyInCollision == true && d.currentlyInCollision == false;
      return result;
   }

   enum class CollisionCenteringType
   {
      Clear,
      Center,
   };

   /*
      static void setCollisionCenter(SessionState& session, CollisionCenteringType type)
      {
      for( uint32_t e = 0; e < session.entityCount; ++e )
      {
      auto setValue = (type == CollisionCenteringType::Clear ? Vec2{} : session.movement[e].position);
      switch( session.collision[e].shape.type )
      {
      case CollisionShape::RectShape:
      {
      session.collision[e].shape.rect.center = setValue;
      } break;
      case CollisionShape::CircleShape:
      {
      session.collision[e].shape.circle.center = setValue;
      } break;
      case CollisionShape::PointShape:
      {
      session.collision[e].shape.point = setValue;
      } break;
      }
      }
      }

      static void toggleCollisionState(SessionState& session)
      {
      for( uint32_t e = 0; e < session.entityCount; ++e )
      {
      session.collision[e].previouslyInCollision = session.collision[e].currentlyInCollision;
      session.collision[e].currentlyInCollision = false;
      }
      }

      static std::vector<CollisionPair> findCollisions(SessionState& session)
      {
      setCollisionCenter(session, CollisionCenteringType::Center);
      toggleCollisionState(session);

      std::vector<CollisionPair> result;
      for( uint32_t e = 0; e < session.entityCount; ++e )
      {
      for( uint32_t r = e + 1; r < session.entityCount; ++r )
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
   struct GameMessage
   {
      enum class Type
      {
         DirectionChange,
         AimChange,
         Fire,
         Ability,
         //#test
         Player_Circle,
         Player_Rect,
         Tree_Circle,
         Tree_Rect,
         Player_Sensor,
      };
      Type type;
      uint32_t entity;
      bool isAnalogue;
      union
      {
         Vec2 direction;
         Vec2 aim;
         bool shooting;
      };
   };

   static std::vector<GameMessage> translateInput_gameplay_session(const EventVector_t& frameEvents)
   {
      std::vector<GameMessage> result;
      for( const auto& e : frameEvents )
      {
         switch( e.type )
         {
            case WE_KEYBOARDKEY:
            {
               GameMessage ge{};
               ge.entity = 0; //player 4
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
                     case Keyboard::_1:
                     {
                        ge.type = GameMessage::Type::Player_Circle;
                        result.push_back(ge);
                     } break;
                     case Keyboard::_2:
                     {
                        ge.type = GameMessage::Type::Player_Rect;
                        result.push_back(ge);
                     } break;
                     case Keyboard::_3:
                     {
                        ge.type = GameMessage::Type::Tree_Circle;
                        result.push_back(ge);
                     } break;
                     case Keyboard::_4:
                     {
                        ge.type = GameMessage::Type::Tree_Rect;
                        result.push_back(ge);
                     } break;
                     case Keyboard::_5:
                     {
                        ge.type = GameMessage::Type::Player_Sensor;
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

            case WE_GAMEPADAXIS:
            {
               GameMessage ge{};
               ge.entity = e.gamepad.id;
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
                     fire.entity = e.gamepad.id;
                     ge.isAnalogue = true;
                     fire.shooting = true;
                     result.push_back(fire);
                  }
               }
            } break;

            case WE_MOUSEBUTTON:
            {
               GameMessage ge{};
               ge.entity = 0; //player 4
               ge.isAnalogue = false;
               ge.type = GameMessage::Type::Fire;
               if( e.mouse.button.id == Mouse::LeftButton )
               {
                  ge.shooting = e.mouse.button.isDown;
                  result.push_back(ge);
               }
            } break;

            case WE_MOUSEMOVE:
            {
               GameMessage ge{};
               ge.entity = 0; //player 4
               ge.isAnalogue = false;
               ge.type = GameMessage::Type::AimChange;
               ge.aim = {(float)e.mouse.position.x, (float)e.mouse.position.y};
               result.push_back(ge);
            } break;
         }
      }
      return result;
   }

   static State update_gameplaySession(DeltaTime time, Constants& constants, GameplayData& state, const EventVector_t& frameEvents, AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx, Camera& camera)
   {
      //    preamble
      //       - update all timers
      advanceTimeForEntities(state.world.deltaTime, time.real);


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
      auto gameEvents = translateInput_gameplay_session(frameEvents);

      std::sort(gameEvents.begin(), gameEvents.end(), [](const GameMessage& l, const GameMessage& r)
      {
         return l.type < r.type;
      });

      for( auto& ge : gameEvents )
      {
         switch( ge.type )
         {
            //       - movement direction target (implies start/stop moving)
            case GameMessage::Type::DirectionChange:
            {
               if( ge.isAnalogue )
               {
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
            //optional cases like opening menus, pausing the game, etc
            //       - open pause menu
            case GameMessage::Type::Player_Circle:
            {
               //                session.collision[0].shape.type = CollisionShape::CircleShape;
               //                session.collision[0].shape.circle = {{}, 1};
            } break;
            case GameMessage::Type::Player_Rect:
            {
               //                session.collision[0].shape.type = CollisionShape::RectShape;
               //                session.collision[0].shape.rect = {{}, {1, 1}};
            } break;
            case GameMessage::Type::Tree_Circle:
            {
               //                session.collision[1].shape.type = CollisionShape::CircleShape;
               //                session.collision[1].shape.circle = {{}, 5};
            } break;
            case GameMessage::Type::Tree_Rect:
            {
               //                session.collision[1].shape.type = CollisionShape::RectShape;
               //                session.collision[1].shape.rect = {{}, {4, 4}};
            } break;
            case GameMessage::Type::Player_Sensor:
            {
               /*               session.collision[0].sensor = !session.collision[0].sensor;*/
            } break;
         }
      }

      //    AI updates:
      //       - decide if should be moving
      //       - decide location to move to, calculate movement direction target
      //       - update aim based on movement direction
      //    simulation updates:
      //       - update movement direction based on target direction
      //       for( uint32_t e = 0; e < session.entityCount; ++e )
      //       {
      //          if( session.movement[e].acceleration > 0.0f )
      //          {
      //             float currentRad = radians(session.movement[e].direction);
      //             auto target = vec2::normalize(session.targetDirection[e]);
      //             float targetRad = radians(target);
      //             float currentDeg = Rad2Deg(currentRad);
      //             float targetDeg = Rad2Deg(targetRad);
      // 
      //             auto wra = targetDeg - currentDeg;
      //             if( wra > 180.0f ) wra -= 360.0f;
      //             if( wra < -180.0f ) wra += 360.0f;
      // 
      //             currentDeg += wra * 4 * session.deltaTime[e].deltaTime;
      //             currentRad = Deg2Rad(currentDeg);
      // 
      //             session.movement[e].direction.x = std::cosf(currentRad);
      //             session.movement[e].direction.y = std::sinf(currentRad);
      //          }
      //       }
      //       - propose movement in current direction
      //       - players, monsters and rockets work the same
      //       - bullets could be made to work the same with some changes to their data
      //       - blasts are a bit more tricky, might have to change how they work entirely

      //       for( uint32_t e = 0; e < session.entityCount; ++e )
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
      //       for( uint32_t e = 0; e < session.entityCount; ++e )
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
      /*for( uint32_t e = 0; e < session.entityCount; ++e )
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
      //       for( uint32_t e = 0; e < session.entityCount; ++e )
      //       {
      //          session.render[e].rotationRadians = radians(session.aim[e].aim);
      //       }
      //       - animation
      //       - transparency
      return State::GameplaySession;
   }

   static void render_gameplaySession(DeltaTime time, GameplayData& session, GameResources assets, GraphicsSystem& gfx, FontSystem& font, /*temp*/ std::vector<Mesh>& meshes)
   {
      gfx.setPerspectiveProjection();
      gfx.setTransparency(true);

      //       for( uint32_t e = 0; e < session.entityCount; ++e )
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



   static bool transition_state(GameData& game)
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

   static bool init_game(GameData& game, AudioSystem& audio, GraphicsSystem& gfx, LuaStack lua)
   {
      bool result = true;
      game.assets = loadGameResources(audio.sounds, gfx.pixelShaders, gfx.vertexShaders, gfx.textures);

      result = checkGameResourcesLoaded(game.assets);

      if( result )
      {
         game.sharedData.meshes.reserve((uint32_t)MeshId::MeshCount);
         //game.sharedData.meshes = loadMeshes(game, gfx.textures);
         //game.sharedData.meshes.push_back({});
         //game.sharedData.meshes[LineMesh] = makeLine({0, 0}, {-1, 0}, game.assets.mainVS, game.assets.mainPS);
         game.sharedData.meshes.emplace_back(makeSolidCircle({}, 1, 32, game.assets.mainVS, game.assets.mainPS));

         game.sharedData.font = loadFont(lua, CORE_RESOURCE("Defs/font.font"), game.assets.font, game.assets.mainVS, game.assets.mainPS);

         game.sharedData.camera.setPosition({0, 0, -50});
         game.sharedData.mousePosition = {0, 0};

         game.constants.playerAcceleration = 60;
         game.constants.playerAimLength = 4;
         game.sharedData.showCursor = false;
         game.sharedData.lockCursor = true;
         game.sharedData.relativeCursor = false;

         game.currentState = State::Startup;
         game.nextState = State::MainMenu;
         result = transition_state(game);
      }

      return true;
   }

   static bool update_game(DeltaTime time, GameData& game, const EventVector_t& frameEvents, AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      bool stillRunning = true;
      enum
      {
         STATE,
         ACTION,
      };

      auto contains = [&frameEvents](Keyboard::Key k, uint32_t s) -> bool
      {
         for( auto e : frameEvents )
         {
            if( e.type == WE_KEYBOARDKEY && e.keyboard.key.id == k && e.keyboard.key.isDown && (s == STATE || e.keyboard.firstTimeDown) )
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

   static void render_game(DeltaTime time, GameData& game, GraphicsSystem& gfx, FontSystem& font)
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
