//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/SessionPerkMenu.h>
/******* C++ headers *******/
#include <functional>
/******* extra headers *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* end headers *******/

namespace Core
{  
   void logic_sessionPerkMenu(RainbowlandGame& game)
   {
      auto evs = game.m_inputSystem.getEvents();
      for( auto& e : evs )
      {
         for( auto& f : game.m_sessionPerkMenuHandlers )
         {
            if( f(e) )
            {
               continue;
            }
         }
      }

      using std::placeholders::_1;

      Transform tPanel;
      Vec2 hPanel;
      Color cPanel;

      tPanel.position.set(0, 0);
      hPanel.set(550, 400);
      cPanel.set(0.1f, 0.1f, 0.1f);
      game.m_renderQueue.enqueueRenderCommand(
         std::bind(drawSolidQuad, _1, hPanel, tPanel, cPanel) );

      generatePerks(game.m_players, game.m_perkDatabase);
      
      for( auto& player : game.m_players )
      {
         Transform tCell;
         Vec2 hCell;

         hCell.set(hPanel.x, hPanel.y / 4);
         tCell.position.set(0, hPanel.y - hCell.y - hCell.y * 2 * player.id);
         tCell.position += tPanel.position;

         game.m_renderQueue.enqueueRenderCommand(
            std::bind(drawHollowQuad, _1, hCell, tCell, Color{}));


         Transform tButtonPart;
         Transform tDescriptionPart;
         Vec2 hButtonPart;
         Vec2 hDescriptionPart;

         hButtonPart.set(hCell.x, hCell.y*0.7f);
         tButtonPart.position.set(0, hCell.y - hButtonPart.y);
         tButtonPart.position += tCell.position;

         game.m_renderQueue.enqueueRenderCommand(
            std::bind(drawHollowQuad, _1, hButtonPart, tButtonPart, Color{}));
         game.m_renderQueue.enqueueRenderCommand(
            std::bind(drawHollowQuad, _1, hDescriptionPart, tDescriptionPart, Color{}));

         //button part
         uint32_t perkIndex = 0;
         Vec2 hButton;
         Color cButton;
         float buttonCellHalfWidth = hButtonPart.x / player.selectablePerks.size();
         
         hButton.set(buttonCellHalfWidth - 10, hButtonPart.y - 10);

         for( auto& perk : player.selectablePerks )
         {
            Transform tButton;
            tButton.position.set(-hButtonPart.x + buttonCellHalfWidth + buttonCellHalfWidth*perkIndex*2, 0);
            tButton.position += tButtonPart.position;
            cButton.set(1, 1, 1);
            if( player.selectedPerkIndex == perkIndex )
            {
               cButton.set(0.98f, 0.92f, 0.01f);
               if( player.chosenPerk != PerkType::PerkTypeCount )
               {
                  cButton.set(0.06f, 0.62f, 0.02f);
               }
            }
            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawHollowQuad, _1, hButton, tButton, cButton));

            tButton.scale *= 0.5f;
            game.m_renderQueue.enqueueRenderCommand(
               std::bind(drawText, _1, game.m_defaultFont, game.m_perkDatabase[perk].name, tButton, cButton,
               TJ_Center, false));
            ++perkIndex;
         }
         
         auto perk = player.selectablePerks[player.selectedPerkIndex];
         
         hDescriptionPart.set(hCell.x, hCell.y*0.3f);
         tDescriptionPart.position.set(0, -hCell.y + hDescriptionPart.y);
         tDescriptionPart.position += tCell.position;
         tDescriptionPart.scale *= 0.5f;
         
         game.m_renderQueue.enqueueRenderCommand(
            std::bind(drawText, _1, game.m_defaultFont, game.m_perkDatabase[perk].description, tDescriptionPart, Color{},
            TJ_Center, false));
         
      }

      if( allPlayersChosePerk(game.m_players) )
      {
         applyPerksForPlayers(game);
         game.m_nextGameState = RainbowlandGame::GS_Session;
      }
      
   }
   
   void render_sessionPerkMenu(RainbowlandGame& game)
   {
      game.m_graphicsSystem.clearCamera();
      game.m_graphicsSystem.setOrthographicProjection();

      game.m_renderQueue.runAndFlushRenderCommands();
   }
}
