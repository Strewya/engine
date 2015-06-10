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
#include "util/time/time_unit_conversions.h"
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
      Bunny1,
      Bunny2,
      Bunny3,
      Bunny1Dino1,
      Bunny2Dino1,
      Bunny3Dino1,
      Bunny1Dino2,
      Bunny2Dino2,
      Bunny3Dino2,
      Penguin,
      BackgroundUpperPart,
      BackgroundLowerPart,
      Count
   };

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

   static std::vector<Mesh> loadMeshes(GameState& game, TextureManager& textures)
   {
      std::vector<Mesh> result{};
      result.resize(MeshId::Count);
      result[Bunny1] = makeMesh(25, 9, 349, 173, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny2] = makeMesh(175, 193, 187, 144, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny3] = makeMesh(8, 347, 383, 140, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny1Dino1] = makeMesh(408, 9, 349, 173, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny2Dino1] = makeMesh(560, 193, 187, 144, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny3Dino1] = makeMesh(393, 347, 383, 140, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny1Dino2] = makeMesh(795, 9, 349, 173, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny2Dino2] = makeMesh(945, 193, 187, 144, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Bunny3Dino2] = makeMesh(778, 347, 383, 140, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[Penguin] = makeMesh(524, 518, 163, 190, textures, game.assets.atlas, game.assets.mainVS, game.assets.mainPS);
      result[BackgroundUpperPart] = makeMesh(0, 0, 1466, 384, textures, game.assets.background, game.assets.mainVS, game.assets.mainPS);
      result[BackgroundLowerPart] = makeMesh(0, 384, 1466, 384, textures, game.assets.background, game.assets.mainVS, game.assets.mainPS);

      return result;
   }

   enum BunnyVariant
   {
      Plain,
      Dino,
      GreyDino,
   };

   struct BunnyData
   {
      float timeFactor;
      Vec2 startingPosition;
      MeshId startingMesh;
      Vec2 impulse;
      Vec2 collisionHalfsize;
      uint32_t variant;
   };

   static bool isDino(uint32_t variant)
   {
      auto result = variant == Dino || variant == GreyDino;
      return result;
   }

   static void makeBunny(SessionState& session, BunnyData bunny)
   {
      session.deltaTime.emplace_back();
      session.position.emplace_back();
      session.render.emplace_back();
      session.movement.emplace_back();
      session.collision.emplace_back();
      session.type.emplace_back();
      session.animation.emplace_back();
      uint32_t e = session.deltaTime.size() - 1;

      session.deltaTime[e] = {0, 0, bunny.timeFactor};
      session.position[e] = {bunny.startingPosition};
      session.render[e] = {Color{1, 1, 1}, bunny.startingMesh};
      session.movement[e] = {60, vec2::normalize(bunny.impulse), bunny.impulse, bunny.startingPosition};
      session.collision[e] = {{CollisionShape::RectShape, {}}, 0, 0x1, 0xffff};
      session.collision[e].shape.rect = {{}, bunny.collisionHalfsize};
      session.type[e] = {bunny.variant};
      session.animation[e] = {0, milisToMicros(100)};

      assert(session.deltaTime.size() == session.position.size() &&
             session.deltaTime.size() == session.render.size() &&
             session.deltaTime.size() == session.movement.size() &&
             session.deltaTime.size() == session.collision.size() &&
             session.deltaTime.size() == session.animation.size() &&
             session.deltaTime.size() == session.type.size());
   }

   static void removeBunny(SessionState& session, uint32_t e)
   {
      session.deltaTime.erase(session.deltaTime.begin() + e);
      session.position.erase(session.position.begin() + e);
      session.render.erase(session.render.begin() + e);
      session.movement.erase(session.movement.begin() + e);
      session.collision.erase(session.collision.begin() + e);
      session.type.erase(session.type.begin() + e);
      session.animation.erase(session.animation.begin() + e);
   }

   static void spawnRandomBunny(GameState& game, SessionState& session)
   {
      Random& rand = game.randGen;
      BunnyData bunny;
      bunny.timeFactor = 1;
      bunny.collisionHalfsize = {1.5f, 1.5f};

      bunny.impulse = {(float)rand.randInt(10, 16), (float)rand.randInt(10, 16)};
      bunny.startingPosition = game.spawnPoint.center +
         Vec2{(float)rand.randInt((int32_t)-game.spawnPoint.halfSize.x, (int32_t)game.spawnPoint.halfSize.x),
         (float)rand.randInt((int32_t)-game.spawnPoint.halfSize.y, (int32_t)game.spawnPoint.halfSize.y)};

      float dinoChance = 0.15f;
      auto chance = rand.randFloat();
      if( chance < dinoChance )
      {
         bunny.variant = rand.randInt(1, 3);
      }
      else
      {
         bunny.variant = 0;
      }
      bunny.startingMesh = (MeshId)(bunny.variant * 3);
      makeBunny(session, bunny);
   }

   static bool session_init(GameState& game, SessionState& session)
   {
      session = SessionState();

      spawnRandomBunny(game, session);

      return true;
   }

   static bool game_init(GameState& game, AudioSystem& audio, GraphicsSystem& gfx, LuaStack lua)
   {
      bool result = true;
      game.assets = loadGameResources(audio.sounds, gfx.pixelShaders, gfx.vertexShaders, gfx.textures);

      result = checkGameResourcesLoaded(game.assets);

      game.meshes = loadMeshes(game, gfx.textures);

      game.camera.setPosition({0, 0, -50});

      game.fontDesc = loadFont(lua, CORE_RESOURCE("font.font"), game.assets.font, game.assets.mainVS, game.assets.mainPS);

      game.dinosCaught = 0;
      game.bunniesCaught = 0;
      game.musicPlaying = false;
      game.spawnBunnyMicros = 0;
      game.spawnBunnyPeriodMicros = milisToMicros(250);
      game.spawnPoint = {{-14, -3}, {10, 2}};
      game.backgroundPos = {511.5f, 198.5f};
      game.penguinPos = {24, -17};

      audio.playMusic(game.assets.song);

      session_init(game, game.session);

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
         else if( two.type == CollisionShape::CircleShape )
         {
            result.colliding = isRectTouchingCircle(one.rect, two.circle);
            result.displacement = getDisplacementRectFromCircle(one.rect, two.circle);
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
                  // #test
                  areInCollision(session.collision[e].shape, session.collision[r].shape);
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
         Click,
         Move,
      };
      Type type;
      uint32_t entity;
      bool isAnalogue;
      union
      {
         Vec2 clickLocation;
         Vec2 move;
      };
   };

   static std::vector<GameMessage> session_translateInput(EventVector_t frameEvents)
   {
      std::vector<GameMessage> result;
      for( const auto& e : frameEvents )
      {
         switch( e.type )
         {
            case WE_MOUSEBUTTON:
            {
               if( e.mouse.button.id == Mouse::LeftButton && e.mouse.button.isDown )
               {
                  GameMessage ge{};
                  ge.clickLocation = {(float)e.mouse.position.x, (float)e.mouse.position.y};
                  result.push_back(ge);
               }
            } break;
            case WE_KEYBOARDKEY:
            {
               GameMessage ge{};
               ge.type = GameMessage::Type::Move;
               float m = 0.5f;
               if( e.keyboard.key.id == Keyboard::ArrowLeft )
               {
                  ge.move = {-m, 0};
                  result.push_back(ge);
               }
               else if( e.keyboard.key.id == Keyboard::ArrowRight )
               {
                  ge.move = {m, 0};
                  result.push_back(ge);
               }
               else if( e.keyboard.key.id == Keyboard::ArrowUp )
               {
                  ge.move = {0, m};
                  result.push_back(ge);
               }
               else if( e.keyboard.key.id == Keyboard::ArrowDown )
               {
                  ge.move = {0, -m};
                  result.push_back(ge);
               }
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
      Vec2 gravity = {0, -9};
      for( uint32_t e = 0; e < session.movement.size(); ++e )
      {
         auto acceleration = vec2::normalize(session.movement[e].velocity) * session.movement[e].acceleration;
         acceleration = gravity;
         session.movement[e].position = acceleration*0.5f*pow2(session.deltaTime[e].deltaTime) + session.movement[e].velocity * session.deltaTime[e].deltaTime + session.position[e].position;
         session.movement[e].velocity = (acceleration)*session.deltaTime[e].deltaTime + session.movement[e].velocity;
      }

      //       - find collisions for new position
      std::vector<uint32_t> clickedBunnies{};
      for( auto& ge : gameEvents )
      {
         if( ge.type == GameMessage::Type::Click )
         {
            CollisionShape point{};
            point.type = CollisionShape::PointShape;
            gfx.setPerspectiveProjection();
            point.point = gfx.screenToWorld(game.camera, ge.clickLocation);
            if( point.point.y >= 0 )
            {
               for( uint32_t e = 0; e < session.collision.size(); ++e )
               {
                  session.collision[e].shape.rect.center = session.movement[e].position;
                  auto result = areInCollision(point, session.collision[e].shape);
                  if( result.colliding )
                  {
                     clickedBunnies.push_back(e);
                  }
                  session.collision[e].shape.rect.center = {};
               }
            }
         }
         else if( ge.type == GameMessage::Type::Move )
         {
            game.penguinPos += ge.move;
         }
      }

      //       - [resolve collisions via displacement vector]->optional depending on performance
      //       - accept new position with displacement vector added
      for( uint32_t e = 0; e < session.movement.size(); ++e )
      {
         if( session.position[e].position != session.movement[e].position )
         {
            session.position[e].position = session.movement[e].position;
         }
      }
      /*for( auto& pair : collisionPairs )
      {
      session.position[pair.collider].position += pair.displacement;
      }*/
      //       - fix camera to new position and zoom level
      //    gameplay update:
      //       - collision response
      // check if bunny should go away because it was clicked on
      for( auto e : clickedBunnies )
      {
         if( isDino(session.type[e].variant) )
         {
            ++game.dinosCaught;
         }
         else
         {
            ++game.bunniesCaught;
         }
         removeBunny(session, e);
      }
      //check if bunny is not visible anymore
      for( uint32_t e = 0; e < session.position.size(); )
      {
         if( session.position[e].position.y + session.collision[e].shape.rect.halfSize.y < -1 &&
            session.position[e].position.x - session.collision[e].shape.rect.halfSize.x > 0 )
         {
            removeBunny(session, e);
         }
         else
         {
            ++e;
         }
      }
      //spawn new bunnies
      game.spawnBunnyMicros += time.deltaMicrosVirt;
      if( game.spawnBunnyMicros >= game.spawnBunnyPeriodMicros )
      {
         game.spawnBunnyMicros -= game.spawnBunnyPeriodMicros;
         spawnRandomBunny(game, session);
      }
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
      //       - animation
      for( uint32_t e = 0; e < session.animation.size(); ++e )
      {
         session.animation[e].micros += session.deltaTime[e].deltaMicros;
         if( session.animation[e].micros >= session.animation[e].microsPerFrame )
         {
            session.animation[e].micros -= session.animation[e].microsPerFrame;
            session.animation[e].frame = (session.animation[e].frame + 1) % 3;
            auto meshId = session.animation[e].frame + 3 * session.type[e].variant;
            session.render[e].mesh = (MeshId)meshId;
         }
      }
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

      session_update(time, game, game.session, frameEvents, audio, lua, gfx);
      return stillRunning;
   }

   static void session_render(Time time, GameState& game, SessionState& session, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.setPerspectiveProjection();
      gfx.setTransparency(false);

      auto scale = Vec2{10, 10};
      auto worldPos = gfx.screenToWorld(game.camera, game.backgroundPos);
      gfx.renderMesh({worldPos, scale}, {}, game.meshes[BackgroundUpperPart]);

      gfx.setTransparency(true);

      for( uint32_t e = 0; e < session.render.size(); ++e )
      {
         gfx.renderMesh({session.position[e].position, {1, 1}, session.render[e].rotationRadians}, {}, game.meshes[session.render[e].mesh]);
      }

      auto spawnMesh = makeOutlineQuad(game.spawnPoint, game.assets.mainVS, game.assets.mainPS);
      gfx.renderMesh({}, {}, spawnMesh);

      gfx.setTransparency(false);
      gfx.renderMesh({-worldPos, scale}, {}, game.meshes[BackgroundLowerPart]);


      gfx.setTransparency(true);
      gfx.renderMesh({game.penguinPos, {2, 2}}, {}, game.meshes[MeshId::Penguin]);
      gfx.setOrthographicProjection();
      std::string str = "Dinolovka!";
      auto caughtMesh = font.makeTextMesh(str.c_str(), game.fontDesc, {1, 1}, Left, Top);
      gfx.renderMesh({Vec2{-500, 360}}, colorFromHex(0xFF7811), caughtMesh);
      str = "Ulovljeno nevinih zecova : " + std::to_string(game.bunniesCaught);
      str += "\nDino ulovljen " + std::to_string(game.dinosCaught) + " puta.";
      caughtMesh = font.makeTextMesh(str.c_str(), game.fontDesc, {1, 1}, Left, Top);
      gfx.renderMesh({Vec2{-100, 360}}, colorFromHex(0xFF7811), caughtMesh);
   }

   static void game_render(Time time, GameState& game, GraphicsSystem& gfx, FontSystem& font)
   {
      gfx.applyCamera(game.camera);


      session_render(time, game, game.session, gfx, font);

   }
}