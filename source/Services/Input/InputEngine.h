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
#include "Services/Input/Event.h"
#include "Services/Input/KeyCodes.h"
#include "Util/Concept.h"
	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Input
{
	enum class DeviceType
	{
		DevKeyboard,
		DevMouse,
		DEVICE_TYPE_COUNT,
	};

	class Engine
	{
	public:
		Engine(Win32::Window& window);
		
		void update();
		bool handle(uint32_t msg, WPARAM wparam, LPARAM lparam);

	private:
		// todo:
		std::list<object_t> m_devices;
		std::list<Event> m_eventQueue;

	};
}
