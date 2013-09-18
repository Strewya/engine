//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/InputEngine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/Input/Mouse.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Input
{
	Engine::Engine(Win32::Window& window)
	{
		using namespace std::placeholders;

		window.registerListener(std::bind(&Engine::handle, this, _1, _2, _3));

		m_devices.push_back(MouseDevice());
	}

	void Engine::update()
	{
		for(auto& event : m_eventQueue)
		{
			
		}
	}

	bool Engine::handle(uint32_t msg, WPARAM wparam, LPARAM lparam)
	{
		//for each device call handle
		for(auto& dev : m_devices)
		{
			Event e;
			if(dev.handle(msg, wparam, lparam, e))
				m_eventQueue.push_back(e);
		}
		return false;
	}
}
