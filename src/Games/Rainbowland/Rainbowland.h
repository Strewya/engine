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
#include <Util/Random.h>
#include <Util/Rect.h>
#include <Util/Time.h>
#include <Util/Transform.h>
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
		
		Time m_logicTimer;
		Time m_renderTimer;
		Time m_deathTimer;
		Time m_gameplayTimer;
		double m_restoreTimeScaleAfterPerkMode;
		
		Camera m_camera;
		Rect m_playingField;
		Random m_randomGenerator;

		std::vector<std::function<bool(const WindowEvent&)>> m_messageHandlers;
		std::vector<Rect> m_splatterImageDatabase;
		std::vector<Rect> m_bonusImageDatabase;
		std::vector<Rect> m_weaponImageDatabase;

		VPlayers m_players;
		VMonsters m_monsters;
		VMonsterSpawners m_monsterSpawners;
		VPickups m_pickups;
		VBullets m_bullets;
		VWeapons m_weaponDatabase;
		VPerks m_perkDatabase;
		VBonuses m_bonusDatabase;
		VSplatters m_splatters;
		
		Circle m_defenseMatrixArea;
		Circle m_timeCapsuleArea;
		bool m_defenseMatrixActive;
		bool m_timeCapsuleActive;
		Time m_defenseMatrixTimer;
		Time m_timeCapsuleTimer;
		uint64_t m_defenseMatrixMicros;
		uint64_t m_timeCapsuleMicros;
		
		uint32_t m_defaultFont;
		uint32_t m_backgroundTexture;
		uint32_t m_charsTexture;
		uint32_t m_splatterTexture;
		uint32_t m_bonusTexture;
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
