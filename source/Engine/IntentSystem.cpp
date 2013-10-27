//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Engine/IntentSystem.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	void IntentSystem::generateIntent(const Intent& i)
	{
		m_intents.push_back(i);
	}

	bool IntentSystem::consumeIntent(uint32_t intentID, Intent& outIntent)
	{
		auto it = std::find_if(m_intents.begin(), m_intents.end(),
			[=](const Intent& i){ return i.intentID == intentID; });

		if(it != m_intents.end())
		{
			outIntent = *it;
			*it = m_intents.back();
			m_intents.pop_back();
			return true;
		}
		return false;
	}
}
