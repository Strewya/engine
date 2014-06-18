#pragma once
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <deque>
#include <vector>
	/*** extra headers if needed ***/
#include <Modules/Input/Service/KeyBindings.h>
#include <Modules/Input/Service/DeviceConcept.h>
#include <Modules/Input/Service/Event.h>
	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Input
{
	class Engine
	{
	public:
		Engine(Win32::Window& window);

		void getEvents(uint64_t maxTimestamp, std::vector<Event>& outEvents);
		
	private:
		std::deque<device_t> m_devices;
		std::deque<Event> m_eventQueue;

		bool handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	};
}
