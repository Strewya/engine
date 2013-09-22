#pragma once
/********************************************
	class:	Input::Context
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
#include <unordered_map>
	/*** end header inclusion ***/

namespace Input
{
	class Event;

	class Context
	{
	public:
		Context();

		uint32_t tryMapping(Event& e) const;

		void addMapping(uint32_t intent, std::function<bool(Event&)> trigger);

	private:
		std::unordered_map<uint32_t, std::function<bool(Event&)>> m_triggers;
	};
}