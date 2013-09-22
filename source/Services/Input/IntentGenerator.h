#pragma once
/********************************************
	class:	IntentGenerator
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include "Services/Input/Context.h"
	/*** end header inclusion ***/

namespace Input
{
	class Event;

	class IntentGenerator
	{
	public:
		IntentGenerator();
		
		uint32_t map(Event& e) const;

		void createContext(const std::string& name);
		Context& getContext(const std::string& name);

	private:
		std::unordered_map<std::string, Context> m_contexts;
		std::list<Context*> m_activeContexts;
	};
}
