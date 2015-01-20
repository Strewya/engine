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
	//************************************ INPUT SYSTEM ************************************//
	bool InputSystem::init(Window& window)
	{
		DEBUG_STATUS(true);

		m_window = &window;

		InitializeInputConstants();

		DEBUG_INIT(InputSystem);
	}

	bool InputSystem::shutdown()
	{
		DEBUG_STATUS(true);

		DEBUG_SHUTDOWN(InputSystem);
	}

	void InputSystem::update(const Clock& clock)
	{
		m_inputEvents.clear();
		WindowEvent we;
		auto currentPeekTime = clock.getCurrentMicros();
		while(m_window->readEvent(currentPeekTime, we))
		{
			m_inputEvents.emplace_back(we);
		}
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
