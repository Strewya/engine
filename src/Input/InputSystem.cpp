//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Input/InputSystem.h>
/******* C++ headers *******/
#include <iostream>
/******* extra headers *******/
#include <Util/Time.h>
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

		std::cout << "InputSystem init " << (status ? "OK" : "FAIL") << std::endl;
		return status;
	}

	bool InputSystem::shutdown()
	{
		bool status = true;

		std::cout << "InputSystem shutdown " << (status ? "OK" : "FAIL") << std::endl;
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
}
