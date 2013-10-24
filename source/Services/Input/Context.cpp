//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Input/Context.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Input
{
	bool equal(const Event& e, const Event& f, bool stateful)
	{
		if(e.device != f.device) return false;
		if(e.type != f.type) return false;
		switch(e.type)
		{
		case EventCode::Axis:
			return e.axis.code == f.axis.code;

		case EventCode::Button:
			return e.button.code == f.button.code && (stateful ? true : e.button.down == f.button.down);
		}
		return false;
	}

	Context::Context()
	{}

	bool Context::mapEvent(const Event& e, Core::Intent& out) const
	{
		for(auto& binding : m_bindings)
		{
			if(equal(binding.first, e, binding.second.type == Core::Intent::Type::State))
			{
				out = binding.second;
				if(e.type == EventCode::Axis)
					out.extraData.range = e.axis.value; //this should go through a converter to normalize the values, etc
				else if(e.type == EventCode::Button)
					out.extraData.state = e.button.down;
				else if(e.type == EventCode::Text)
					out.extraData.symbol = e.text;
				return true;
			}
		}
		return false;
	}

	void Context::addAction(const Event& e, uint32_t intentID)
	{
		m_bindings.emplace_back( std::make_pair( e, Core::Intent() ) );
		m_bindings.back().second.intentID = intentID;
		m_bindings.back().second.type = Core::Intent::Type::Action;
	}

	void Context::addRange(const Event& e, uint32_t intentID)
	{
		if(e.type == EventCode::Axis)
		{
			m_bindings.emplace_back( std::make_pair( e, Core::Intent() ) );
			m_bindings.back().second.intentID = intentID;
			m_bindings.back().second.type = Core::Intent::Type::Range;
		}
	}

	void Context::addState(const Event& e, uint32_t intentID)
	{
		if(e.type != EventCode::Axis)
		{
			m_bindings.emplace_back( std::make_pair( e, Core::Intent() ) );
			m_bindings.back().second.intentID = intentID;
			m_bindings.back().second.type = Core::Intent::Type::State;
		}
	}
}