#pragma once
/********************************************
	class:	IntentGenerator
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include <list>
	/*** end header inclusion ***/

namespace Core
{
	struct Intent
	{
		enum class Type
		{
			Action,
			Range,
			State
		};
		uint32_t intentID;
		InstanceID target;
		Type type;
		union
		{
			bool state;
			double range;
			wchar_t symbol;
		} extraData;
	};

	class IntentSystem
	{
	public:
		IntentSystem();
		
		void generateIntent(const Intent& i);
		bool consumeIntent(uint32_t intentID, Intent& outIntent);
		
	private:
		std::list<Intent> m_intents;
	};
}
