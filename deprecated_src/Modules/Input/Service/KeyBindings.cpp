//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Modules/Input/Service/KeyBindings.h>
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
	/*** end headers ***/

namespace Input
{
	void KeyBindings::createContext(const std::string& contextName)
	{
		if(m_contexts.find(contextName) == m_contexts.end())
		{
			m_contexts.emplace(contextName, Context());
		}
	}

	Context& KeyBindings::getContext(const std::string& contextName)
	{
		auto it = m_contexts.find(contextName);
		if(it == m_contexts.end())
		{
			throw std::runtime_error("No key binding context exists with specified name" + contextName);
		}
		return it->second;
	}

	void KeyBindings::pushContext(const std::string& contextName, uint32_t priority)
	{
		auto it = m_contexts.find(contextName);
		if(it != m_contexts.end())
		{
			m_activeContexts.emplace(priority, &it->second);
		}
	}

	void KeyBindings::popContext(const std::string contextName)
	{
		auto it = m_contexts.find(contextName);
		if(it != m_contexts.end())
		{
			Context* context = &it->second;
			auto check = [&](std::pair<uint32_t, Context*> pair){ return context == pair.second; };
			auto searchResult = std::find_if(m_activeContexts.begin(), m_activeContexts.end(), check);
			if(searchResult != m_activeContexts.end())
			{
				m_activeContexts.erase(searchResult);
			}
		}
	}

	bool KeyBindings::findBinding(const Event& e, Core::Intent& out) const
	{
		auto check = [&](std::pair<uint32_t,Context*> pair){ return pair.second->mapEvent(e, out); };
		return std::any_of(m_activeContexts.begin(), m_activeContexts.end(), check);
	}
}
