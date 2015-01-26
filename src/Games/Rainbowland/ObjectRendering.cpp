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
         game.m_graphicsSystem.v3_setInstanceData({splatter.transform}, {splatter.color});
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
      game.m_graphicsSystem.v3_setInstanceData({{}}, {{0.6f, 0.6f, 0.6f}});
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
         Rect r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_players + obj.id];
         float ratio = r.halfWidth / r.halfHeight;
         r.center /= atlasSize;
         r.halfWidth /= atlasSize.x;
         r.halfHeight /= atlasSize.y;

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color);

         Transform tHP{obj.transform.position + Vec2f{0, 1}, {0.03f, 0.03f}, 0};
         std::string hpExp = std::to_string(obj.health);
         drawText(game.m_graphicsSystem, game.m_defaultFont, hpExp, tHP, obj.color, TJ_Center, false);

         auto time = abilityTimeLeft(game, obj.ability);
         auto text = std::to_string(time);
         tHP.position = obj.transform.position + Vec2f{0, -1};
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, tHP, obj.color, TJ_Center, false);


         Transform tDir{obj.transform.position + obj.direction * 2, {0.3f, 0.3f}, 0};
         drawSolidCircle(game.m_graphicsSystem, 1, 8, tDir, obj.color);
      }
#if 0
      if( game.m_players.size() > 0 )
      {
         std::vector<Transform> tfs;
         std::vector<Color> fills;
         tfs.reserve(game.m_players.size());
         fills.reserve(game.m_players.size());
         for( auto& obj : game.m_players )
         {
            tfs.emplace_back(obj.transform);
            auto col = obj.color;
            fills.emplace_back(col);
         }

         auto inds = game.m_graphicsSystem.v3_makeHollowCircleIndices(18);
         auto verts = game.m_graphicsSystem.v3_makeCircleVertices(game.m_players.front().collisionData.center,
                                                                  game.m_players.front().collisionData.radius, 18);
         game.m_graphicsSystem.v3_setVertices(verts);
         game.m_graphicsSystem.v3_setIndices(inds);
         game.m_graphicsSystem.v3_setInstanceData(tfs, fills);
         game.m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
         game.m_graphicsSystem.v3_draw(inds.size(), game.m_players.size());
      }
#endif
   }

   void draw_monsters(RainbowlandGame& game, Vec2f atlasSize)
   {
      for( auto& obj : game.m_monsters )
      {
         Rect r;
         switch( obj.type )
         {
            case Flower:
            {
               r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_flower];
            } break;

            case Butterfly:
            {
               r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_butterfly];
            } break;

            case Ladybug:
            {
               r = game.m_rainbowlandImageDatabase[game.m_imageStartIndex_ladybug];
            } break;
         }

         float ratio = r.halfWidth / r.halfHeight;
         r.center /= atlasSize;
         r.halfWidth /= atlasSize.x;
         r.halfHeight /= atlasSize.y;

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, r, {ratio, 1}, obj.transform, obj.color);

#if 0
         auto t = obj.transform;
         t.position += obj.collisionData_attack.center;
         drawHollowCircle(game.m_graphicsSystem, obj.collisionData_attack.radius, 18, t, {});
         t = obj.transform;
         t.position += obj.collisionData_hitbox.center;
         drawHollowCircle(game.m_graphicsSystem, obj.collisionData_hitbox.radius, 18, t, {});
#endif
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
            drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, circleRect, {1.5f, 1.5f}, pickup.transform, pickup.color);
         else
            scale.set(2.0f, 2.0f);
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, imageRect, scale, pickup.transform, pickup.color);
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
         game.m_graphicsSystem.v3_setInstanceData({t}, {{}});
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
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, {});
         auto displayTime = turretTimeLeft(game);
         auto text = std::to_string(displayTime);
         t.scale.set(0.03f, 0.03f);
         t.position.y -= 1;
         drawText(game.m_graphicsSystem, game.m_defaultFont, text, t, {0, 0, 0}, TJ_Left, false);
      }
      if( game.m_blink.active )
      {
         Transform t{game.m_blink.area.center, {1, 1}, 0};
         drawHollowCircle(game.m_graphicsSystem, game.m_blink.area.radius, 18, t, {0.7f, 0.7f, 0});
         t.position = game.m_blink.target;
         drawHollowCircle(game.m_graphicsSystem, game.m_blink.area.radius, 18, t, {0.7f, 0.7f, 0});
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
         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, rect, {ratio, 1}, t, {});
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

         drawTexturedQuad(game.m_graphicsSystem, game.m_atlasTexture, img, hs, t, c);
         //drawHollowCircle(m_graphicsSystem, obj.area.radius, 36, t, c);
      }
   }

   void draw_gui(RainbowlandGame& game, Vec2f atlasSize)
   {

   }
}
