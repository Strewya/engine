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
/******* extra headers *******/
#include "input/gamepad.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "util/collision_checks.h"
/******* end headers *******/

namespace core
{
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

   enum MeshId
   {
      YellowPlayer,
      BluePlayer,
      GreenPlayer,
      PurplePlayer,
      RedPlayer,
   };

   static std::vector<Mesh> loadMeshes(GameState& game, TextureManager& textures)
   {
      std::vector<Mesh> result{};
      //players
      loadMeshBundle(1795, 420, 3085, 673, 5, 1, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS, result);

      return result;
   }

   static bool game_init(GameState& game, AudioSystem& audio, GraphicsSystem& gfx, LuaStack lua)
   {
      bool result = true;
      game.assets = loadGameResources(audio.sounds, gfx.pixelShaders, gfx.vertexShaders, gfx.textures);

      result = checkGameResourcesLoaded(game.assets);

      game.meshes = loadMeshes(game, gfx.textures);

      game.camera.setPosition({0, 0, -50});
      game.globalGameState = GameState::GlobalGameState::MainMenu;
      game.gameplayState = GameState::GameplayState::ClassPick;

      game.fontDesc = loadFont(lua, CORE_RESOURCE("Defs/font.font"), game.assets.font, game.assets.mainVS, game.assets.mainPS);

      game.constants.playerAcceleration = 60;
      game.constants.playerAimLength = 4;

      return true;
   }

   static void makePlayer(SessionState& session)
   {
      session.deltaTime.push_back({0, 0, 1});
      session.position.push_back({Vec2{0, 0}});
      session.render.push_back({Color{0, 1, 0}, GreenPlayer});
      session.movement.push_back({0, Vec2{0, 0}, Vec2{0, 0}});
      session.aim.push_back({Vec2{0, 0}});
      session.targetDirection.push_back({0, 0});
      session.collision.push_back({{CollisionShape::CircleShape, {}}, 0, 0x1, 0xffff});
      session.collision.back().shape.circle = {{}, 1};

      assert(session.deltaTime.size() == session.position.size() &&
             session.deltaTime.size() == session.render.size() &&
             session.deltaTime.size() == session.movement.size() &&
             session.deltaTime.size() == session.aim.size() &&
             session.deltaTime.size() == session.collision.size() &&
             session.deltaTime.size() == session.targetDirection.size());
   }

   static bool session_init(GameState& game, SessionState& session)
   {
      session = SessionState();
      makePlayer(session);

      return true;
   }

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

   struct CollisionCheckResult
   {
      Vec2 displacement;
      bool colliding;
   };

   static CollisionCheckResult areInCollision(CollisionShape one, CollisionShape two)
   {
      CollisionCheckResult result{};
      result.colliding = false;
      if( one.type > two.type )
      {
         auto varSwitch = one;
         one = two;
         two = varSwitch;
      }
      if( one.type == CollisionShape::PointShape )
      {
         if( two.type == CollisionShape::CircleShape )
         {
            result.colliding = isPointInsideCircle(one.point, two.circle);
            result.displacement = getDisplacementPointFromCircle(one.point, two.circle);
         }
         else if( two.type == CollisionShape::RectShape )
         {
            result.colliding = isPointInsideRect(one.point, two.rect);
            result.displacement = getDisplacementPointFromRect(one.point, two.rect);
         }
      }
      else if( one.type == CollisionShape::CircleShape )
      {
         if( two.type == CollisionShape::CircleShape )
         {
            result.colliding = isCircleTouchingCircle(one.circle, two.circle);
            result.displacement = getDisplacementCircleFromCircle(one.circle, two.circle);
         }
         else if( two.type == CollisionShape::RectShape )
         {
            result.colliding = isCircleTouchingRect(one.circle, two.rect);
            result.displacement = getDisplacementCircleFromRect(one.circle, two.rect);
         }
      }
      else if( one.type == CollisionShape::RectShape )
      {
         if( two.type == CollisionShape::RectShape )
         {
            result.colliding = isRectTouchingRect(one.rect, two.rect);
            result.displacement = getDisplacementRectFromRect(one.rect, two.rect);
         }
      }
      return result;
   }

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

   static std::vector<CollisionPair> findCollisions(SessionState& session)
   {
      for( uint32_t e = 0; e < session.collision.size(); ++e )
      {
         switch( session.collision[e].shape.type )
         {
            case CollisionShape::RectShape:
            {
               session.collision[e].shape.rect.center = session.movement[e].position;
            } break;
            case CollisionShape::CircleShape:
            {
               session.collision[e].shape.circle.center = session.movement[e].position;
            } break;
            case CollisionShape::PointShape:
            {
               session.collision[e].shape.point = session.movement[e].position;
            } break;
         }
      }

      std::vector<CollisionPair> result;
      for( uint32_t e = 0; e < session.collision.size(); ++e )
      {
         for( uint32_t r = e + 1; r < session.collision.size(); ++r )
         {
            bool testCollision = shouldCollide(session.collision[e], session.collision[r]);

            if( testCollision )
            {
               auto testResult = areInCollision(session.collision[e].shape, session.collision[r].shape);
               if( testResult.colliding )
               {
                  result.push_back({e, r, testResult.displacement});
               }
            }
         }
      }

      for( uint32_t e = 0; e < session.collision.size(); ++e )
      {
         switch( session.collision[e].shape.type )
         {
            case CollisionShape::RectShape:
            {
               session.collision[e].shape.rect.center = {};
            } break;
            case CollisionShape::CircleShape:
            {
               session.collision[e].shape.circle.center = {};
            } break;
            case CollisionShape::PointShape:
            {
               session.collision[e].shape.point = {};
            } break;
         }
      }
      return result;
   }

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

   static std::vector<GameMessage> session_translateInput(EventVector_t frameEvents)
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

   static bool session_update(Time time, GameState& game, SessionState& session, const EventVector_t& frameEvents, AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
   {
      //    preamble
      //       - update all timers
      for( auto& dt : session.deltaTime )
      {
         dt.deltaMicros = static_cast<uint32_t>(time.deltaMicrosVirt*dt.factor);
         dt.deltaTime = time.deltaTimeVirt*dt.factor;
      }

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
      auto gameEvents = session_translateInput(frameEvents);

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
                  session.targetDirection[ge.entity] = ge.direction;
               }
               else
               {
                  session.targetDirection[ge.entity] += ge.direction;
               }
               session.movement[ge.entity].acceleration = game.constants.playerAcceleration*vec2::length(vec2::normalize(session.targetDirection[ge.entity]));
            } break;
            //       - aim
            case GameMessage::Type::AimChange:
            {
               if( ge.isAnalogue )
               {
                  session.aim[ge.entity].aim = ge.aim*game.constants.playerAimLength;
               }
               else
               {
                  auto window = Vec2{game.constants.windowWidth, game.constants.windowHeight}*0.5f;
                  gfx.setPerspectiveProjection();
                  auto aim = session.aim[ge.entity].aim;
                  aim = gfx.worldToScreen(game.camera, aim);
                  aim += ge.aim;
                  aim = gfx.screenToWorld(game.camera, aim);
                  if( vec2::length2(aim) > pow2(game.constants.playerAimLength) )
                  {
                     session.aim[ge.entity].aim = vec2::setLength(aim, game.constants.playerAimLength);
                  }
                  else
                  {
                     session.aim[ge.entity].aim = aim;
                  }
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
         }
      }

      //    AI updates:
      //       - decide if should be moving
      //       - decide location to move to, calculate movement direction target
      //       - update aim based on movement direction
      //    simulation updates:
      //       - update movement direction based on target direction
      for( uint32_t e = 0; e < session.targetDirection.size(); ++e )
      {
         if( session.movement[e].acceleration > 0.0f )
         {
            float currentRad = radians(session.movement[e].direction);
            auto target = vec2::normalize(session.targetDirection[e]);
            float targetRad = radians(target);
            float currentDeg = Rad2Deg(currentRad);
            float targetDeg = Rad2Deg(targetRad);

            auto wra = targetDeg - currentDeg;
            if( wra > 180.0f ) wra -= 360.0f;
            if( wra < -180.0f ) wra += 360.0f;

            currentDeg += wra * 4 * session.deltaTime[e].deltaTime;
            currentRad = Deg2Rad(currentDeg);

            session.movement[e].direction.x = std::cosf(currentRad);
            session.movement[e].direction.y = std::sinf(currentRad);
         }
      }
      //       - propose movement in current direction
      //       - players, monsters and rockets work the same
      //       - bullets could be made to work the same with some changes to their data
      //       - blasts are a bit more tricky, might have to change how they work entirely
      for( uint32_t e = 0; e < session.movement.size(); ++e )
      {
         auto acceleration = session.movement[e].direction * session.movement[e].acceleration;
         acceleration += -session.movement[e].velocity * 10.0f;
         session.movement[e].position = acceleration*0.5f*pow2(session.deltaTime[e].deltaTime) + session.movement[e].velocity * session.deltaTime[e].deltaTime + session.movement[e].position;
         session.movement[e].velocity = acceleration*session.deltaTime[e].deltaTime + session.movement[e].velocity;
      }

      //       - find collisions for new position
      auto collisionPairs = findCollisions(session);
      //       - [resolve collisions via displacement vector]->optional depending on performance
      //       - accept new position with displacement vector added
      for( uint32_t e = 0; e < session.movement.size(); ++e )
      {
         session.position[e].position = session.movement[e].position;
      }
      for( auto& pair : collisionPairs )
      {
         session.position[pair.collider].position += pair.displacement;
      }
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
      for( uint32_t e = 0; e < session.render.size(); ++e )
      {
         session.render[e].rotationRadians = radians(session.aim[e].aim);
      }
      //       - animation
      //       - transparency
      return true;
   }

   static bool game_update(Time time, GameState& game, const EventVector_t& frameEvents, AudioSystem& audio, LuaStack lua, GraphicsSystem& gfx)
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

      switch( game.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               game.globalGameState = GameState::GlobalGameState::Gameplay;
               game.gameplayState = GameState::GameplayState::ClassPick;
            }
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            switch( game.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  if( contains(Keyboard::Space, ACTION) )
                  {
                     game.gameplayState = GameState::GameplayState::Session;
                     session_init(game, game.session);
                  }
               } break;
               case GameState::GameplayState::Session:
               {
                  session_update(time, game, game.session, frameEvents, audio, lua, gfx);

                  if( contains(Keyboard::Space, ACTION) )
                  {
                     game.globalGameState = GameState::GlobalGameState::Score;
                  }
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            if( contains(Keyboard::Space, ACTION) )
            {
               game.globalGameState = GameState::GlobalGameState::MainMenu;
            }
         } break;
      }

      return stillRunning;
   }

   static void session_render(Time time, GameState& game, SessionState& session, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.setPerspectiveProjection();
      gfx.setTransparency(true);

      for( uint32_t e = 0; e < session.render.size(); ++e )
      {
         gfx.renderMesh({session.position[e].position, {1, 1}, session.render[e].rotationRadians}, {}, game.meshes[session.render[e].mesh]);
         auto aimMesh = makeSolidCircle({}, 0.2f, 8, game.assets.mainVS, game.assets.mainPS);
         gfx.renderMesh({session.position[e].position + session.aim[e].aim}, session.render[e].color, aimMesh);
         auto collisionMesh = makeOutlineCircle(session.collision[e].shape.circle.center, session.collision[e].shape.circle.radius, 16, game.assets.mainVS, game.assets.mainPS);
         gfx.renderMesh({session.position[e].position}, session.render[e].color, collisionMesh);
      }
   }

   static void game_render(Time time, GameState& game, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.applyCamera(game.camera);
      /*
      systems.gfx->setPerspectiveProjection();
      systems.gfx->setTransparency(false);
      systems.gfx->renderMesh(Transform{}, Color{}, state.backgroundMesh);
      systems.gfx->setTransparency(true);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position}, Color{}, state.playerMesh);
      systems.gfx->renderMesh(Transform{state.movingThings[0].position + state.movingThings[0].direction, {0.1f, 0.1f}}, Color{}, makeSolidQuad({}, {1, 1}, assets.mainVS, assets.mainPS));
      */

      std::string caption, subcaption;
      switch( game.globalGameState )
      {
         case GameState::GlobalGameState::MainMenu:
         {
            caption = "Main menu";
         } break;
         case GameState::GlobalGameState::Gameplay:
         {
            caption = "Gameplay";
            switch( game.gameplayState )
            {
               case GameState::GameplayState::ClassPick:
               {
                  subcaption = "\nClass picking";
               } break;
               case GameState::GameplayState::Session:
               {
                  subcaption = "\nSession";

                  session_render(time, game, game.session, gfx, font);
               } break;
            }
         } break;
         case GameState::GlobalGameState::Score:
         {
            caption = "Score screen";
         } break;
      }

      gfx.setOrthographicProjection();
      gfx.setTransparency(true);
      caption += (time.deltaMicrosVirt == 0 ? " (paused)" : " (running)");
      caption += subcaption;
      auto mesh = font.makeTextMesh(caption.c_str(), game.fontDesc, {1, 1}, Left, Top);
      gfx.renderMesh(Vec2{-game.constants.windowWidth*0.5f, game.constants.windowHeight*0.5f}, {}, mesh);
   }
}