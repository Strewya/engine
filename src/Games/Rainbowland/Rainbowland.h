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
#include <Caches/FontCache.h>
#include <Caches/ImageCache.h>
#include <Caches/TextureCache.h>
#include <Games/Rainbowland/GameObjects.h>
#include <Games/Rainbowland/GuiSystem.h>
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Scripting/LuaSystem.h>
#include <Util/Clock.h>
#include <Util/Rect.h>
#include <Util/Timers.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;
	class WindowEvent;
	
	class RainbowlandGame
	{
	public:
		GraphicsSystem m_graphicsSystem;
		InputSystem m_inputSystem;
		LuaSystem m_luaSystem;
		GuiSystem m_guiSystem;

		FontCache m_fontCache;
		ImageCache m_imageCache;
		TextureCache m_textureCache;
		
		Clock m_logicClock; //drives the global logic time
		Clock m_renderClock; //drives the global render time
		IncrementingTimer m_gameplayTimer; //drives the general gameplay
		DecrementingTimer m_deathTimer; //decrementing timer for the deatch scenario
		double m_restoreTimeScaleAfterPerkMode;
		std::vector<double> m_timeScaleForGameplayTimer;
		
		Camera m_camera;
		Rect m_playingField;

		std::vector<std::function<bool(const WindowEvent&)>> m_messageHandlers;

		VPlayers m_players;
		VMonsters m_monsters;
		VMonsterSpawners m_monsterSpawners;
		VPickups m_pickups;
		VRayBullets m_rayBullets;
		VWeapons m_weaponDatabase;
		VPerks m_perkDatabase;
		VBonuses m_bonusDatabase;
		
		uint32_t m_defaultFont;
		Window* m_window;
		bool m_isRunning;
		bool perkMode;
		

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);
	};
}
