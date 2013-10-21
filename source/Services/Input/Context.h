#pragma once
/********************************************
	class:	Input::Context
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
#include <deque>
	/*** extra headers if needed ***/
#include "Engine/IntentSystem.h"
#include "Services/Input/Event.h"
	/*** end header inclusion ***/

namespace Input
{
	class Context
	{
	public:
		Context();

		bool mapEvent(const Event& e, Core::Intent& out) const;

		void addAction(const Event& e, uint32_t intentID);
		void addRange(const Event& e, uint32_t intentID);
		void addState(const Event& e, uint32_t intentID);
		

	private:
		std::deque< std::pair< Event, Core::Intent > > m_bindings;
	};
}