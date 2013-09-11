//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/InputEngine.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Input
{
	Engine::Engine()
		: m_keys(Keyboard::_KeyCount, false), m_mouseKeys(Mouse::_KeyCount, false)
	{
	}

	void Engine::update()
	{
		for(auto& event : m_eventQueue)
		{
			switch(event.type)
			{
			case EventType::KeyPressed:
				m_keys[event.key.code] = true;
				m_keys[Keyboard::_Alt] = event.key.alt;
				m_keys[Keyboard::_Control] = event.key.control;
				m_keys[Keyboard::_Shift] = event.key.shift;
			break;

			case EventType::KeyReleased:
				m_keys[event.key.code] = false;
				m_keys[Keyboard::_Alt] = event.key.alt;
				m_keys[Keyboard::_Control] = event.key.control;
				m_keys[Keyboard::_Shift] = event.key.shift;
			break;

			case EventType::MouseButtonPressed:
				m_mouseKeys[event.mouseButton.button] = true;
			break;

			case EventType::MouseButtonReleased:
				m_mouseKeys[event.mouseButton.button] = false;
			break;

			default:
			break;
			}

			for(auto trigger : m_triggers)
			{
				if(trigger.second(event))
				{
					m_outboundIntents.push_back(trigger.first);
				}
			}
		}
	}

	void Engine::purgeEvents()
	{
		m_eventQueue.clear();
	}

	std::list<Event>& Engine::getEventQueue()
	{
		return m_eventQueue;
	}

	bool Engine::isPressed(Keyboard::Keys key) const
	{
		if(key>=0 && key<Keyboard::_KeyCount)
			return m_keys[key];
		return false;
	}

	bool Engine::isPressed(Mouse::Keys button) const
	{
		if(button>=0 && button<Mouse::_KeyCount)
			return m_mouseKeys[button];
		return false;
	}

	bool Engine::eatEvent(Event& out)
	{
		if(!m_eventQueue.empty())
		{
			out = m_eventQueue.front();
			m_eventQueue.pop_front();
			return true;
		}
		return false;
	}




	Engine::InputTrigger spawnTriggerKeyboardDown(uint32_t key, bool shift, bool ctrl, bool alt)
	{
		return [=](Event& e)
		{
			if(e.type == EventType::KeyPressed &&
				e.key.code == key &&
				e.key.alt == alt &&
				e.key.shift == shift &&
				e.key.control == ctrl)
			{
				return true;
			}
			return false;
		};
	}

	Engine::InputTrigger spawnTriggerKeyboardUp(uint32_t key)
	{
		return [=](Event& e)
		{
			if(e.type == EventType::KeyPressed &&
				e.key.code == key)
			{
				return true;
			}
			return false;
		};
	}

	Engine::InputTrigger spawnTriggerMouseDown(uint32_t key)
	{
		return [=](Event& e)
		{
			if(e.type == EventType::MouseButtonPressed &&
				e.mouseButton.button == key)
			{
				return true;
			}
			return false;
		};
	}

	Engine::InputTrigger spawnTriggerMouseUp(uint32_t key)
	{
		return [=](Event& e)
		{
			if(e.type == EventType::MouseButtonReleased &&
				e.mouseButton.button == key)
			{
				return true;
			}
			return false;
		};
	}

	void Engine::registerTrigger(uint32_t intentCode, DeviceType device, uint32_t key, bool isContinuous)
	{
		switch(device)
		{
		case DeviceType::Keyboard:
			m_triggers.insert(std::make_pair(intentCode, spawnTriggerKeyboardDown(key, false, false, false)));
			if(isContinuous)
				m_triggers.insert(std::make_pair(intentCode, spawnTriggerKeyboardUp(key)));
			break;

		case DeviceType::Mouse:
			m_triggers.insert(std::make_pair(intentCode, spawnTriggerMouseDown(key)));
			if(isContinuous)
				m_triggers.insert(std::make_pair(intentCode, spawnTriggerMouseUp(key)));
			break;

		default:
			break;
		}
	}
}
