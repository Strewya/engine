//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	void RainbowlandGame::shutdown()
	{

	}

	bool RainbowlandGame::init(Window& window)
	{
		m_window = &window;

		bool initStatus =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_fontCache.init(m_textureCache) &&
			m_textureCache.init(m_graphicsSystem) &&
			true;

		window.resize(800, 600);
		window.showCursor(true);

		return initStatus;
	}

	bool RainbowlandGame::tick()
	{
		return true;
	}
}
