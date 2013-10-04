//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Engine.h"
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

		window.registerListener(std::bind(&Engine::handle, this, _1, _2, _3, _4));

		m_devices.push_back(MouseDevice());

		m_activeContexts.emplace_back(&m_contexts["main"]);
	}

	void Engine::update()
	{
		//if an event wasn't mapped to an intent, then we simply discard it
		//this is why the event is pulled out from the queue
		while(!m_eventQueue.empty())
		{
			Event e = m_eventQueue.front();
			m_eventQueue.pop_front();
			for(auto it : m_activeContexts)
			{
				if(it->mapEvent(e))
				{
					//what do i do here? o.O
					break;
				}
			}
		}
	}

	bool Engine::handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
	{
		//for each device call handle
		for(auto& dev : m_devices)
		{
			if(dev.handle(hwnd, msg, wparam, lparam, m_eventQueue))
			{
				//we return immediately as there should be only one device capable of handling
				//any specific message
				return true;
			}
		}
		return false;
	}

	Context& Engine::getContext(const std::string& name)
	{
		return m_contexts[name];
	}

	void Engine::pushContext(const std::string& name)
	{
		m_activeContexts.push_back(&m_contexts[name]);
	}

	void Engine::popContext()
	{
		m_activeContexts.pop_back();
	}

}
