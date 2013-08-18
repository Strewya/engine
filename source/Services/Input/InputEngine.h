#pragma once
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include <list>
#include <vector>
	/*** extra headers if needed ***/
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Input
{
	class Engine
	{
	public:
		Engine();
		
		void Update();
		void PurgeEvents();

		std::list<Event>& getEventQueue();

		bool isPressed(Keyboard::Keys key) const;
		bool isPressed(Mouse::Keys button) const;
		bool PollEvent(Event& out);

	private:
		std::vector<bool> _keys;
		std::vector<bool> _mouseKeys;

		std::list<Event> _eventQueue;

	};
}
