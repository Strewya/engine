#pragma once
/********************************************
	class:	IntentGenerator
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Core
{
	struct IntentData
	{
		std::set<uint32_t> states;
		std::set<uint32_t> actions;
		std::unordered_map<uint32_t, double> ranges;
	};

	class IntentSystem
	{
	public:
		typedef std::function<void(IntentData& intents)> Callback;
		IntentSystem();
		
		void addState(uint32_t intentCode, bool state);
		void addAction(uint32_t intentCode);
		void addRange(uint32_t intentCode, double value);
		
		void clear();
		void dispatch();

	private:
	};
}
