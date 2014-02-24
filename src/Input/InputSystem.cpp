//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Input/InputSystem.h>
/******* C++ headers *******/
#include <iostream>
/******* extra headers *******/
#include <Input/KeyCodes.h>
#include <Util/Time.h>
#include <Util/Utility.h>
#include <Window/Window.h>
#include <Window/WindowEvent.h>
/******* end headers *******/

namespace Core
{
	static EventVector_t gatherInput(uint64_t currentTimeMicros, Window& window)
	{
		EventVector_t evs;
		evs.reserve(20);
		WindowEvent we;
		auto pwe = &we;
		while(window.peekEvent(currentTimeMicros, we))
		{
			evs.emplace_back(we);
		}
		return evs;
	}


	//************************************ INPUT SYSTEM ************************************//
	bool InputSystem::init(Window& window)
	{
		bool status = true;

		m_window = &window;

		InitializeInputConstants();

		DEBUG_INFO("InputSystem init ", status ? "OK" : "FAIL");
		return status;
	}

	bool InputSystem::shutdown()
	{
		bool status = true;

		DEBUG_INFO("InputSystem shutdown ", status ? "OK" : "FAIL");
		return status;
	}

	void InputSystem::update(const Time& timer)
	{
		m_inputEvents = gatherInput(timer.getCurMicros(), *m_window);
	}

	const EventVector_t& InputSystem::getEvents() const
	{
		return m_inputEvents;
	}

	uint32_t InputSystem::getEventCount() const
	{
		return m_inputEvents.size();
	}

	WindowEvent InputSystem::getEvent(uint32_t index) const
	{
		if(index < m_inputEvents.size())
			return m_inputEvents[index];
		return WindowEvent{0, 0};
	}
}
