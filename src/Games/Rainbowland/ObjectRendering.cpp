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
   void draw_splatters_to_texture(RainbowlandGame& game, Vec2f atlasSize)
   {
      //draw splatters to texture render target
      game.m_graphicsSystem.v3_setTextureAsRenderTarget();
      
      auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();
      game.m_graphicsSystem.setTransparencyMode(true);
      game.m_graphicsSystem.v3_setTexture(game.m_atlasTexture);
      for( auto splatter : game.m_splatters )
      {
         auto sdb = game.m_rainbowlandImageDatabase[splatter.splatterImage];
         float ratio = sdb.halfWidth / sdb.halfHeight;
         sdb.halfWidth /= atlasSize.x;
         sdb.halfHeight /= atlasSize.y;
         sdb.center /= atlasSize;
         auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
         vertices[0].setTextureCoords(sdb.left(), sdb.bottom());
         vertices[1].setTextureCoords(sdb.right(), sdb.bottom());
         vertices[2].setTextureCoords(sdb.left(), sdb.top());
         vertices[3].setTextureCoords(sdb.right(), sdb.top());

         game.m_graphicsSystem.v3_setVertices(vertices);
         game.m_graphicsSystem.v3_setIndices(indices);
         game.m_graphicsSystem.v3_setInstanceData({splatter.transform}, {splatter.color}, {0});
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         game.m_graphicsSystem.v3_draw(indices.size(), 1);
      }

      game.m_splatters.clear();
      game.m_graphicsSystem.v3_clearTextureAsRenderTarget();
   }

   void draw_background(RainbowlandGame& game, Vec2f atlasSize)
   {
      auto vertices = game.m_graphicsSystem.v3_makeQuadVertices({}, game.m_playingField.halfSize());
      vertices[0].setTextureCoords(0, 0);
      vertices[1].setTextureCoords(1, 0);
      vertices[2].setTextureCoords(0, 1);
      vertices[3].setTextureCoords(1, 1);
      auto indices = game.m_graphicsSystem.v3_makeSolidQuadIndices();

      game.m_graphicsSystem.v3_setVertices(vertices);
      game.m_graphicsSystem.v3_setIndices(indices);
      game.m_graphicsSystem.v3_setInstanceData({{}}, {{0.6f, 0.6f, 0.6f}}, {0});
      game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      game.m_graphicsSystem.setTransparencyMode(false);
      game.m_graphicsSystem.v3_setTexture(game.m_backgroundTexture);
      game.m_graphicsSystem.v3_draw(indices.size(), 1);

      //draw the texture render target over the screen
      game.m_graphicsSystem.setTransparencyMode(true);
      game.m_graphicsSystem.v3_setTextureFromRenderTarget();
      game.m_graphicsSystem.v3_draw(indices.size(), 1);
      
   }

   void draw_players(RainbowlandGame& game, Vec2f atlasSize)
   {
      for( auto& obj : game.m_players )
      {
         game.m_graphicsSystem.togglePixelShader();
         Rect r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_healthBar];
         float ratio = r.halfWidth / r.halfHeight;
         r.center /= atlasSize;
         r.halfWidth /= atlasSize.x;
         r.halfHeight /= atlasSize.y;

         Transform t = obj.transform;
         t.rotation = 0;
         
         //health circle
         t.scale *= 1.2f;
         Color c = obj.color;
         //c.clip(0.8f);
         float health = (float)obj.health;
         health /= obj.maxHealth;
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, t, c, health);
         game.m_graphicsSystem.togglePixelShader();

         r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_players + obj.id];
         ratio = r.halfWidth / r.halfHeight;
         r.center /= atlasSize;
         r.halfWidth /= atlasSize.x;
         r.halfHeight /= atlasSize.y;
         //player
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color, {0});

         Transform tDir{obj.transform.position + obj.direction * 2, {0.2f, 0.2f}, 0};
         drawSolidCircle(game.m_graphicsSystem, 1, 8, tDir, obj.color);
      }
   }

   void draw_monsters(RainbowlandGame& game, Vec2f atlasSize)
   {
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
         float ratio = r.halfWidth / r.halfHeight;
         r.center /= atlasSize;
         r.halfWidth /= atlasSize.x;
         r.halfHeight /= atlasSize.y;

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color, {0});
      }
   }

   void draw_pickups(RainbowlandGame& game, Vec2f atlasSize)
   {
      for( auto& pickup : game.m_pickups )
      {
         auto imageRect = game.m_rainbowlandImageDatabase[pickup.iconIndex];
         auto circleRect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_bonuses + 6];

         imageRect.center /= atlasSize;
         imageRect.halfWidth /= atlasSize.x;
         imageRect.halfHeight /= atlasSize.y;
         circleRect.center /= atlasSize;
         circleRect.halfWidth /= atlasSize.x;
         circleRect.halfHeight /= atlasSize.y;

         Vec2f scale{1, 1};
         if( pickup.bonus != BonusTypeCount )
            drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, circleRect, {1.5f, 1.5f}, pickup.transform, pickup.color, {0});
         else
            scale.set(2.0f, 2.0f);
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, imageRect, scale, pickup.transform, pickup.color, {0});
      }
   }

   void draw_skills(RainbowlandGame& game, Vec2f atlasSize)
   {
      if( game.m_defenseMatrix.active )
      {
         Transform t{game.m_defenseMatrix.area.center, {1, 1}, 0};
         drawSolidCircle(game.m_graphicsSystem, game.m_defenseMatrix.area.radius, 36, t, {0.25f, 0.42f, 0.76f, 0.2f});
         auto displayTime = defenseMatrixTimeLeft(game);
         auto text = std::to_string(displayTime);
         t.scale.set(0.03f, 0.03f);
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
      }
      if( game.m_timeCapsule.active )
      {
         Transform t{game.m_timeCapsule.area.center, {1, 1}, 0};
         auto vertices = game.m_graphicsSystem.v3_makeCircleVertices({}, game.m_timeCapsule.area.radius, 36);
         auto indices = game.m_graphicsSystem.v3_makeSolidCircleIndices(36);
         for( auto& v : vertices )
         {
            v.setDiffuse(0.75f, 0.42f, 0.2f, 0.0f);
         }
         vertices[0].setDiffuse(0.75f, 0.42f, 0.2f, 0.8f);

         game.m_graphicsSystem.v3_setVertices(vertices);
         game.m_graphicsSystem.v3_setIndices(indices);
         game.m_graphicsSystem.v3_setInstanceData({t}, {{}}, {0});
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         game.m_graphicsSystem.v3_draw(indices.size(), 1);

         auto displayTime = timeCapsuleTimeLeft(game);
         auto text = std::to_string(displayTime);
         t.scale.set(0.03f, 0.03f);
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
      }
      if( game.m_turret.active )
      {
         Transform t{game.m_turret.area.center, {1, 1}, 0};
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_turret];
         float ratio = rect.halfWidth / rect.halfHeight;
         rect.center /= atlasSize;
         rect.halfHeight /= atlasSize.y;
         rect.halfWidth /= atlasSize.x;
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, {}, {0});
         auto displayTime = turretTimeLeft(game);
         auto text = std::to_string(displayTime);
         t.scale.set(0.03f, 0.03f);
         t.position.y -= 1;
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
      }
      if( game.m_healingCloud.active )
      {
         Transform t{game.m_healingCloud.area.center, {1, 1}, 0};
         drawHollowCircle(game.m_graphicsSystem, game.m_healingCloud.area.radius, 18, t, {0.7f, 0.7f, 0});
         auto displayTime = healingCloudTimeLeft(game);
         auto text = std::to_string(displayTime);
         t.scale.set(0.03f, 0.03f);
         t.position.y -= 1;
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
      }
   }

   void draw_damagers(RainbowlandGame& game, Vec2f atlasSize)
   {
      for( auto& obj : game.m_bullets )
      {
         drawLine(game.m_graphicsSystem, obj.trail, {1, 1, 1, 0}, obj.position, {1, 1, 1, 1}, {}, {});
      }
      for( auto& obj : game.m_rockets )
      {
         Rect rect = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_rocket];
         float ratio = rect.halfWidth / rect.halfHeight;
         rect.center /= atlasSize;
         rect.halfWidth /= atlasSize.x;
         rect.halfHeight /= atlasSize.y;

         Transform t{obj.body.center, {1, 1}, std::atan2(obj.direction.y, obj.direction.x)};
         t.rotation = Deg2Rad(Rad2Deg(t.rotation) - 90);
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, {}, {0});
      }
      for( auto& obj : game.m_blasts )
      {
         Transform t{obj.area.center, {1, 1}, obj.rotation};
         Color c{1, 1, 1};
         Rect img = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_blast];
         img.center /= atlasSize;
         img.halfWidth /= atlasSize.x;
         img.halfHeight /= atlasSize.y;

         Vec2f hs{obj.area.radius*1.1f, obj.area.radius*1.1f};

         float percent = obj.area.radius / obj.maxRadius;
         if( percent > 0.8f )
         {
            percent = 1 - percent;
            c.a *= (percent / (0.2f));
         }

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, img, hs, t, c, {0});
         //drawHollowCircle(m_graphicsSystem, obj.area.radius, 36, t, c);
      }
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
