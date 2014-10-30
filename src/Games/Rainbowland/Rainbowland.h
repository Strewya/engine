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
#include <Graphics/Camera.h>
#include <Graphics/GraphicsSystem.h>
#include <Input/InputSystem.h>
#include <Util/Time.h>
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

		FontCache m_fontCache;
		ImageCache m_imageCache;
		TextureCache m_textureCache;
		
		Time m_logicTimer;
		Time m_renderTimer;

		Camera m_camera;
		Rect m_playingField;

		std::vector<std::function<bool(const WindowEvent&)>> m_messageHandlers;

		std::vector<Player> m_players;
		std::vector<Monster> m_monsters;
		std::vector<Bonus> m_bonuses;
		std::vector<RayBullet> m_rayBullets;
		
		Window* m_window;
		uint32_t m_numPlayers;
		bool m_isRunning;
		

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);
	};
}
