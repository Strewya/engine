//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/IntentGenerator.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/input/Event.h"
	/*** end headers ***/

namespace Input
{
	IntentGenerator::IntentGenerator()
	{
	}

	uint32_t IntentGenerator::map(Event& e) const
	{
		uint32_t intent = 0;
		for(auto* context : m_activeContexts)
		{
			intent = context->tryMapping(e);
			if(intent != 0)
				break;
		}
		return 0;
	}

	void IntentGenerator::createContext(const std::string& name)
	{
		m_contexts.emplace(name, Context());
	}

	Context& IntentGenerator::getContext(const std::string& name)
	{
		auto it = m_contexts.find(name);
		if(it == m_contexts.end())
		{
			throw std::runtime_error("Attempt to return non existing InputContext");
		}
		return it->second;
	}
}
