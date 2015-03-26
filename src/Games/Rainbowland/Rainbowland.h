#pragma once
/********************************************
*	class:	RainbowlandGame
*	usage:
********************************************/
/******* C++ headers *******/
#include <array>
#include <cstdint>
#include <functional>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Audio/AudioSystem.h>
#include <Caches/FontCache.h>
#include <Caches/ImageCache.h>
#include <Caches/TextureCache.h>
#include <Games/Rainbowland/GameDefinitions.h>
#include <Games/Rainbowland/GuiSystem.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Graphics/RenderQueue.h>
#include <Input/InputSystem.h>
#include <Scripting/LuaSystem.h>
#include <Util/Random.h>
#include <Util/Rect.h>
#include <Util/Time.h>
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
   class Window;
   class WindowEvent;

   class RainbowlandGame
   {
   public:
      typedef std::function<bool(const WindowEvent&)> EventHandlerFunction;
      typedef std::vector<EventHandlerFunction> EventHandlerList;


      GraphicsSystem m_graphicsSystem;
      AudioSystem m_audioSystem;
      InputSystem m_inputSystem;
      LuaSystem m_luaSystem;

      RenderQueue m_renderQueue;

      FontCache m_fontCache;
      ImageCache m_imageCache;
      TextureCache m_textureCache;

      Clock m_logicTimer;
      Clock m_renderTimer;
      CooldownTimer m_deathTimer;
      Clock m_gameplayTimer;
      PeriodicTimer m_difficultyTimer;
      double m_currentTimeScale;

      Camera m_camera;
      Vec2f m_cameraBounds;
      Rect m_playingField;
      Random m_randomGenerator;

      SessionPreparationData m_preparationData;

      EventHandlerList m_messageHandlers;

      EventHandlerList m_sessionPreparationHandlers;
      EventHandlerList m_sessionHandlers;
      EventHandlerList m_sessionPerkMenuHandlers;
      EventHandlerList m_sessionEndHandlers;

      std::vector<Rect> m_rainbowlandImageDatabase;

      uint32_t m_imageStartIndex_flower;
      uint32_t m_imageStartIndex_ladybug;
      uint32_t m_imageStartIndex_splatter;
      uint32_t m_imageStartIndex_butterfly;
      uint32_t m_imageStartIndex_blast;
      uint32_t m_imageStartIndex_crosshair;
      uint32_t m_imageStartIndex_bonuses;
      uint32_t m_imageStartIndex_players;
      uint32_t m_imageStartIndex_rocket;
      uint32_t m_imageStartIndex_turret;
      uint32_t m_imageStartIndex_gradient;
      uint32_t m_imageStartIndex_healthBar;
      uint32_t m_imageStartIndex_mouseControls;
      uint32_t m_imageStartIndex_wsadControls;
      uint32_t m_imageStartIndex_gamepadControls;

      std::vector<uint32_t> ladybugAnimationLoop;
      std::vector<uint32_t> flowerAnimationLoop;
      std::vector<uint32_t> butterflyAnimationLoop;

      VWeapons m_weaponDatabase;
      VPerks m_perkDatabase;
      VBonuses m_bonusDatabase;
      VMonsterSpawners m_monsterSpawners;
      MonsterObjectPool m_monsterPool;
      VPlayers m_players;
      VPlayers m_deadPlayers;
      VPMonsters m_monsters;
      VPickups m_pickups;
      VBullets m_bullets;
      VBlasts m_blasts;
      VRockets m_rockets;
      VSplatters m_splatters;
      VActiveBonuses m_activeBonuses;
      Grid m_monsterGrid;

      DefenseMatrix m_defenseMatrix;
      TimeCapsule m_timeCapsule;
      HealingCloud m_healingCloud;
      Turret m_turret;

      float m_difficulty;
      uint32_t m_playerCount;
      uint32_t m_killCounter;
      uint32_t m_totalKillCount;
      uint32_t m_highScore;
      uint32_t m_maxMonsterCount;

      uint32_t m_experience;
      uint32_t m_experienceIncrement;
      uint32_t m_experienceForNextLevel;
      uint32_t m_level;
      int32_t m_flavour;

      CooldownTimer m_perkModeTransitionTimer;
      bool m_enteringPerkMode;
      bool m_exitingPerkMode;

      uint32_t m_defaultFont;
      uint32_t m_atlasTexture;
      uint32_t m_backgroundTexture;

      void* m_shootSounds[WeaponTypeCount];
      void* m_reloadSfx;

      Window* m_window;
      bool m_isRunning;

      enum GameState
      {
         GS_SessionPreparation,
         GS_Session,
         GS_SessionPerkMenu,
         GS_SessionEnd,
         GS_Count
      };
      GameState m_currentGameState;
      GameState m_nextGameState;


      bool init(Window& window);
      void shutdown();

      bool tickLogic(uint64_t updateTime);
      void tickRender(uint64_t updateTime);
   };
}
