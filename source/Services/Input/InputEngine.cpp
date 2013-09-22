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

		window.registerListener(std::bind(&Engine::handle, this, _1, _2, _3, _4));

		m_devices.push_back(MouseDevice());

		//load some config from file and create the required contexts
		m_intentGenerator.createContext("main");
		Context& mainContext = m_intentGenerator.getContext("main");
		//mainContext.addMapping(1, [](){});
	}

	std::vector<uint32_t>&& Engine::update()
	{
		std::vector<uint32_t> generatedIntents;
		generatedIntents.reserve(m_eventQueue.size());
		//if an event wasn't mapped to an intent, then we simply discard it
		//this is why the event is pulled out from the queue
		while(!m_eventQueue.empty())
		{
			Event e = m_eventQueue.front();
			m_eventQueue.pop_front();
			/*
			here we pass the event to the context based intent generator class
			if and intent is generated, it should return non-zero
			zero should be reserved to always be "unmapped"
			*/
			uint32_t intent = 0;//m_intentGenerator.map(e);
			if(intent != 0)
			{
				generatedIntents.push_back(intent);
			}
		}
		generatedIntents.swap(generatedIntents);
		return std::move(generatedIntents);
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
}
