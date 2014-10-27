#pragma once
/********************************************
*	class:	RainbowlandGame
*	usage:
********************************************/
/******* C++ headers *******/
#include <stdint.h>
/******* common headers *******/
/******* extra headers *******/
#include <Caches/FontCache.h>
#include <Caches/TextureCache.h>
#include <Graphics/GraphicsSystem.h>
/******* end header inclusion *******/

namespace Core
{
	class Window;
	
	class RainbowlandGame
	{
	public:
		GraphicsSystem m_graphicsSystem;
		FontCache m_fontCache;
		TextureCache m_textureCache;
		Window* m_window;
		

		bool init(Window& window);
		bool tick();
		void shutdown();

		bool tickLogic(uint64_t updateTime);
		void tickRender(uint64_t updateTime);
	};
}
