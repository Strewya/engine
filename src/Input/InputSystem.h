#pragma once
/********************************************
*	class:	InputSystem
*	usage:
********************************************/
/******* C++ headers *******/
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Window/WindowEvent.h>
/******* end header inclusion *******/

namespace Core
{
	class Time;
	class Window;

	typedef std::vector<WindowEvent> EventVector_t;

	class InputSystem
	{
	public:
		bool init(Window& window);
		bool shutdown();

		void update(const Time& timer);

		const EventVector_t& getEvents() const;

	private:
		Window* m_window;
		EventVector_t m_inputEvents;
	};
}
