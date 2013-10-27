//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Engine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/Input/MouseDevice.h"
#include "Win32/Window.h"
	/*** end headers ***/

namespace Input
{
	Engine::Engine(Win32::Window& window)
	{
		using namespace std::placeholders;

		window.registerListener(std::bind(&Engine::handle, this, _1, _2, _3, _4));

		m_devices.push_back(MouseDevice());
	}

	bool Engine::handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
	{
		auto check = [&](device_t& dev){ return dev.handle(hwnd, msg, wparam, lparam, m_eventQueue); };
		return std::any_of(m_devices.begin(), m_devices.end(), check);
	}

	bool Engine::nextEvent(Event& e)
	{
		if(m_eventQueue.empty())
		{
			return false;
		}
		e = m_eventQueue.front();
		m_eventQueue.pop_front();
		return true;
	}
}
