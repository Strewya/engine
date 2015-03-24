//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/ObjectRendering.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
#include <Util/Vec2.h>
/******* end headers *******/

namespace Core
{
   void applyTextureCoords(VertexBuffer& vertices, Rect image)
   {
      vertices[0].setTextureCoords(image.left(), image.bottom());
      vertices[1].setTextureCoords(image.right(), image.bottom());
      vertices[2].setTextureCoords(image.left(), image.top());
      vertices[3].setTextureCoords(image.right(), image.top());
   }

   float makeTextureCoordinatesAndGetRatio(Rect& image, Vec2f atlasSize)
   {
      float ratio = image.halfWidth / image.halfHeight;
      image.halfWidth /= atlasSize.x;
      image.halfHeight /= atlasSize.y;
      image.center /= atlasSize;
      return ratio;
   }

   void draw_splatters(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      game.m_graphicsSystem.setTransparencyMode(true);
      
      auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
      
      //draw splatters to texture render target
      game.m_graphicsSystem.v3_setTextureAsRenderTarget();
      
      for( auto splatter : game.m_splatters )
      {
         auto sdb = game.m_rainbowlandImageDatabase[splatter.splatterImage];
         float ratio = makeTextureCoordinatesAndGetRatio(sdb, atlasSize);
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         applyTextureCoords(vertices, sdb);

         game.m_graphicsSystem.v4_setData(vertices, indices, splatter.transform, splatter.color, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();
      game.m_splatters.clear();

      //draw the texture render target over the screen
      game.m_graphicsSystem.v3_clearTextureAsRenderTarget();
      game.m_graphicsSystem.v3_setTextureFromRenderTarget();
      auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, game.m_playingField.halfSize());
      vertices[0].setTextureCoords(0, 0);
      vertices[1].setTextureCoords(1, 0);
      vertices[2].setTextureCoords(0, 1);
      vertices[3].setTextureCoords(1, 1);

      game.m_graphicsSystem.v4_setData(vertices, indices, {}, {0.6f, 0.6f, 0.6f}, 0);
      game.m_graphicsSystem.v4_drawBuffers();
      
   }

   void draw_background(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_backgroundTexture);
      game.m_graphicsSystem.setTransparencyMode(false);
      
      auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
      auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, game.m_playingField.halfSize());
      vertices[0].setTextureCoords(0, 0);
      vertices[1].setTextureCoords(1, 0);
      vertices[2].setTextureCoords(0, 1);
      vertices[3].setTextureCoords(1, 1);

      game.m_graphicsSystem.v4_setData(vertices, indices, {}, {0.6f, 0.6f, 0.6f}, 0);
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_players(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      //health circle
      game.m_graphicsSystem.togglePixelShader();
      for( auto& obj : game.m_players )
      {
         Rect r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_healthBar];
         float ratio = makeTextureCoordinatesAndGetRatio(r, atlasSize);

         Transform t = obj.transform;
         t.rotation = 0;
         t.scale *= 1.2f;
         float health = ((float)obj.health) / obj.maxHealth;
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         applyTextureCoords(vertices, r);

         game.m_graphicsSystem.v4_setData(vertices, indices, t, obj.color, health);
      }
      game.m_graphicsSystem.v4_drawBuffers();

      game.m_graphicsSystem.togglePixelShader();
      for( auto& obj : game.m_players )
      {
         Rect r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_players + obj.id];
         float ratio = makeTextureCoordinatesAndGetRatio(r, atlasSize);
         Transform tDir{obj.transform.position + obj.direction * 2, {0.2f, 0.2f}, 0};
      
         //player
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         applyTextureCoords(vertices, r);
         game.m_graphicsSystem.v4_setData(vertices, indices, obj.transform, obj.color, 0);
         //direction circle
         vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, 1, 8);
         indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(8);
         game.m_graphicsSystem.v4_setData(vertices, indices, tDir, obj.color, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_monsters(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      game.m_graphicsSystem.setTransparencyMode(true);

      for( auto* obj_ptr : game.m_monsters )
      {
         auto& obj = *obj_ptr;
         uint32_t imageIndex = UINT32_MAX;
         switch( obj.type )
         {
            case Flower:
            {
               imageIndex = game.flowerAnimationLoop[obj.currentAnimationFrame];
            } break;

            case Butterfly:
            {
               imageIndex = game.butterflyAnimationLoop[obj.currentAnimationFrame];
            } break;

            case Ladybug:
            {
               imageIndex = game.ladybugAnimationLoop[obj.currentAnimationFrame];
            } break;
         }
         assert(imageIndex != UINT32_MAX);
         Rect r = game.m_rainbowlandImageDatabase[imageIndex];
         float ratio = makeTextureCoordinatesAndGetRatio(r, atlasSize);

         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});

         //top and bottom are reversed because texture coordinates y axis is reversed
         applyTextureCoords(vertices, r);

         game.m_graphicsSystem.v4_setData(vertices, indices, obj.transform, obj.color, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_pickups(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      game.m_graphicsSystem.setTransparencyMode(true);
      for( auto& pickup : game.m_pickups )
      {
         auto imageRect = game.m_rainbowlandImageDatabase[pickup.iconIndex];
         auto circleRect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_bonuses + 6];

         makeTextureCoordinatesAndGetRatio(imageRect, atlasSize);
         makeTextureCoordinatesAndGetRatio(circleRect, atlasSize);
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();

         Vec2f scale{1, 1};
         if( pickup.bonus != BonusTypeCount )
         {
            auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {1.5f, 1.5f});
            applyTextureCoords(vertices, circleRect);
            game.m_graphicsSystem.v4_setData(vertices, indices, pickup.transform, pickup.color, 0);
         }
         else
         {
            scale.set(2.0f, 2.0f);
         }
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, scale);
         applyTextureCoords(vertices, imageRect);
         game.m_graphicsSystem.v4_setData(vertices, indices, pickup.transform, pickup.color, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_skills(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      game.m_graphicsSystem.setTransparencyMode(true);
      //skill outlines
      if( game.m_defenseMatrix.active )
      {
         Transform t{game.m_defenseMatrix.area.center, {1, 1}, 0};
         auto indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(36);
         auto vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, game.m_defenseMatrix.area.radius, 36);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, {0.25f, 0.42f, 0.76f, 0.2f}, 0);
      }
      if( game.m_timeCapsule.active )
      {
         Transform t{game.m_timeCapsule.area.center, {1, 1}, 0};
         auto indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(36);
         auto vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, game.m_timeCapsule.area.radius, 36);
         for( auto& v : vertices )
         {
            v.setDiffuse(0.75f, 0.42f, 0.2f, 0.0f);
         }
         vertices[0].setDiffuse(0.75f, 0.42f, 0.2f, 0.8f);

         game.m_graphicsSystem.v4_setData(vertices, indices, t, {}, 0);
      }
      if( game.m_turret.active )
      {
         Transform t{game.m_turret.area.center, {1, 1}, 0};
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_turret];
         float ratio = makeTextureCoordinatesAndGetRatio(rect, atlasSize);
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         applyTextureCoords(vertices, rect);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, {}, 0);
      }
      if( game.m_healingCloud.active )
      {
         Transform t{game.m_healingCloud.area.center, {1, 1}, 0};
         auto indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(18);
         auto vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, game.m_healingCloud.area.radius, 18);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, {0.0f, 0.7f, 0, 0.2f}, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();

      //skill durations
      game.m_graphicsSystem.v3_setFontTexture(game.m_defaultFont);
      auto drawer = [](uint32_t displayTime, Transform t, RainbowlandGame& game)
      {
         auto text = std::to_string(displayTime);
         auto vertices = game.m_graphicsSystem.v3_makeTextVertices(game.m_defaultFont, text, false);
         auto indices = game.m_graphicsSystem.v3_makeTextIndices(text.size());
         t.scale.set(0.03f, 0.03f);
         t = game.m_graphicsSystem.justifyText(t, game.m_graphicsSystem.textHalfSize(game.m_defaultFont, text).x * 2, TJ_Center);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, {0, 0, 0}, 0);
      };
      if( game.m_defenseMatrix.active )
      {
         auto displayTime = defenseMatrixTimeLeft(game);
         Transform t{game.m_defenseMatrix.area.center, {1, 1}, 0};
         drawer(displayTime, t, game);
      }
      if( game.m_timeCapsule.active )
      {
         auto displayTime = timeCapsuleTimeLeft(game);
         Transform t{game.m_timeCapsule.area.center, {1, 1}, 0};
         drawer(displayTime, t, game);
      }
      if( game.m_turret.active )
      {
         auto displayTime = turretTimeLeft(game);
         Transform t{game.m_turret.area.center, {1, 1}, 0};
         drawer(displayTime, t, game);
      }
      if( game.m_healingCloud.active )
      {
         auto displayTime = healingCloudTimeLeft(game);
         Transform t{game.m_healingCloud.area.center, {1, 1}, 0};
         drawer(displayTime, t, game);
      }
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_damagers(RainbowlandGame& game, Vec2f atlasSize)
   {
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
      for( auto& obj : game.m_bullets )
      {
         std::vector<Vertex> vertices
         {
            {obj.trail.x, obj.trail.y, 0, 1, 1, 1, 0, -1, -1},
            {obj.position.x, obj.position.y, 0, 1, 1, 1, 1, -1, -1}
         };
         std::vector<uint32_t> indices{0, 1};
         game.m_graphicsSystem.v4_setData(vertices, indices, {}, {}, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();

      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      game.m_graphicsSystem.setTransparencyMode(true);
      for( auto& obj : game.m_rockets )
      {
         Transform t{obj.body.center, {1, 1}, std::atan2(obj.direction.y, obj.direction.x)};
         t.rotation = Deg2Rad(Rad2Deg(t.rotation) - 90);
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_rocket];
         float ratio = makeTextureCoordinatesAndGetRatio(rect, atlasSize);

         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         applyTextureCoords(vertices, rect);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, {}, 0);
      }
      for( auto& obj : game.m_blasts )
      {
         Transform t{obj.area.center, {1, 1}, obj.rotation};
         Color c{1, 1, 1};
         Rect img = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_blast];
         makeTextureCoordinatesAndGetRatio(img, atlasSize);

         Vec2f hs{obj.area.radius*1.1f, obj.area.radius*1.1f};

         float percent = obj.area.radius / obj.maxRadius;
         if( percent > 0.8f )
         {
            percent = 1 - percent;
            c.a *= (percent / (0.2f));
         }
         
         auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, hs);
         applyTextureCoords(vertices, img);
         game.m_graphicsSystem.v4_setData(vertices, indices, t, c, 0);
      }
      game.m_graphicsSystem.v4_drawBuffers();
   }

   void draw_gui(RainbowlandGame& game, Vec2f atlasSize)
   {

   }

   void draw_grid_debug_info(Grid& grid, GraphicsSystem& gfx)
   {
      auto x = grid.cellHalfsize.x*(grid.columns / 2) - grid.cellHalfsize.x;
      auto y = grid.cellHalfsize.y*(grid.rows / 2);
      for( float i = -x; i <= x; i += (grid.cellHalfsize.x * 2) )
      {
         drawLine(gfx, {i, y}, {i, -y}, {}, {});
      }
      for( float i = -y; i <= y; i += (grid.cellHalfsize.y * 2) )
      {
         drawLine(gfx, {x, i}, {-x, i}, {}, {});
      }

      x += grid.cellHalfsize.x;
      y += grid.cellHalfsize.y;
      for( float j = -y; j < y; j += (grid.cellHalfsize.y*2) )
      {
         for( float i = -x; i < x; i += (grid.cellHalfsize.x*2) )
         {
            auto cellCoords = calculateCellCoords(grid, Vec2f{i, j});
            auto cellIndex = indexFromCellCoords(grid, cellCoords);
            for( auto* monster : grid.cells[cellIndex].contents )
            {
               drawLine(gfx, monster->transform.position, {i, j}, {}, {});
            }
         }
      }
   }
}
