#pragma once
/********************************************
	class:	IntentGenerator
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include <deque>
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

		Intent() {}
		Intent(uint32_t intentID, Type type) : intentID(intentID), type(type) {}
	};

	class IntentSystem
	{
	public:
		
		void generateIntent(const Intent& i);
		bool consumeIntent(uint32_t intentID, Intent& outIntent);
		
	private:
		std::deque<Intent> m_intents;
	};
}
