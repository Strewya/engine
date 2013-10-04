#pragma once
/********************************************
	class:	Input::Context
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
#include <unordered_map>
	/*** extra headers if needed ***/
#include "Services/Input/Event.h"
	/*** end header inclusion ***/

namespace Input
{
	class Context
	{
	public:
		typedef std::function<void(double)> AxisCallback;
		typedef std::function<void(void)> ButtonCallback;

		Context();

		void insertCallback(Event& e, ButtonCallback& c);
		void insertCallback(Event& e, AxisCallback& c);
		
		bool mapEvent(Event& e);

	private:
		std::list< std::pair<Event, ButtonCallback> > m_buttonCallbacks;
		std::list< std::pair<Event, AxisCallback>   > m_axisCallbacks;
	};

	bool equal(Event& e, Event& f);
}