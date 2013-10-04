//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Context.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/input/Event.h"
	/*** end headers ***/

namespace Input
{
	Context::Context()
	{}

	void Context::insertCallback(Event& e, ButtonCallback& c)
	{
		m_buttonCallbacks.push_back( std::make_pair(e, c) );
	}

	void Context::insertCallback(Event& e, AxisCallback& c)
	{
		m_axisCallbacks.push_back( std::make_pair(e, c) );
	}

	bool Context::mapEvent(Event& e)
	{
		for(auto it : m_buttonCallbacks)
		{
			if(equal(e, it.first))
			{
				it.second();
				return true;
			}
		}

		for(auto it : m_axisCallbacks)
		{
			if(equal(e, it.first))
			{
				it.second(e.axis.value);
				return true;
			}
		}

		return false;
	}

	bool equal(Event& e, Event& f)
	{
		if(e.device != f.device) return false;
		if(e.type != f.type) return false;
		switch(e.type)
		{
		case EventCode::Axis:
			return e.axis.code == f.axis.code;

		case EventCode::Button:
			return e.button.code == f.button.code && e.button.down == f.button.down;
		}
		return false;
	}
}