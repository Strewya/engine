#pragma once
/********************************************
	class:	InputSystem
	usage:	Concrete input system implementation to create input
			events based on injected data
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include <functional>
#include <list>
#include <unordered_map>
#include <vector>
	/*** extra headers if needed ***/
#include "Services/Input/Context.h"
#include "Services/Input/DeviceConcept.h"
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Input
{
	class Engine
	{
	public:
		Engine(Win32::Window& window);
		
		void update();
		Context& getContext(const std::string& name);
		void pushContext(const std::string& name);
		void popContext();

	private:
		std::list<device_t> m_devices;
		std::list<Event> m_eventQueue;

		std::unordered_map<std::string, Context> m_contexts;
		std::list<Context*> m_activeContexts;
		
		bool handle(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	};
}
