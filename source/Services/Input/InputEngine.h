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
	/*** end header inclusion ***/

namespace Win32 { class Window; }

namespace Input
{
	enum class DeviceType
	{
		Keyboard,
		Mouse,
		DEVICE_TYPE_COUNT,
	};

	class Engine
	{
	public:
		Engine();
		
		void update();
		void purgeEvents();

		std::list<Event>& getEventQueue();

		bool isPressed(Keyboard::Keys key) const;
		bool isPressed(Mouse::Keys button) const;
		bool eatEvent(Event& out);

	private:
		// todo:
		// std::list/map<Device> m_devices;
		std::vector<bool> m_keys;
		std::vector<bool> m_mouseKeys;

		std::list<Event> m_eventQueue;


		//the next set of data should be in a input translator or some such class/concept
	public:
		typedef std::function<bool(Event&)> InputTrigger;
	
		std::unordered_map<uint32_t, InputTrigger> m_triggers;
		std::list<uint32_t> m_outboundIntents;
	
		void registerTrigger(uint32_t intentCode, DeviceType device, uint32_t key, bool isContinuous);
	};
}
