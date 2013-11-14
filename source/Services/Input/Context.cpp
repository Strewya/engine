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
	bool match(const Event& e, const Event& f, const Core::Intent& intent)
	{
		if(e.device != f.device) return false;
		if(e.type != f.type) return false;
		switch(e.type)
		{
		case EventCode::Axis:
			return e.axis.code == f.axis.code;

		case EventCode::Button:
			return (e.button.code == f.button.code) &&
				(intent.type == Core::Intent::Type::State ? true : e.button.down == f.button.down);

		case EventCode::Text:
			return true;
		}
		return false;
	}

	Context::Context()
	{}

	bool Context::mapEvent(const Event& e, Core::Intent& out) const
	{
		for(auto& binding : m_bindings)
		{
			if(match(e, binding.first, binding.second))
			{
				out = binding.second;
				switch(e.type)
				{
				case EventCode::Axis:
				{
					out.extraData.range = e.axis.value;
					auto it = m_converters.find(out.msgId);
					if(it != m_converters.end())
					{
						out.extraData.range = it->second.convert(out.extraData.range);
					}
					break;
				}

				case EventCode::Button:
					out.extraData.state = e.button.down;
					break;

				case EventCode::Text:
					out.extraData.symbol = e.text;
					break;
				}
				return true;
			}
		}
		return false;
	}

	void Context::addAction(uint32_t intentID, const Event& e)
	{
		m_bindings.emplace_back( std::make_pair( e, Core::Intent(intentID, Core::Intent::Type::Action) ) );
	}

	void Context::addRange(uint32_t intentID, const Event& e)
	{
		if(e.type == EventCode::Axis)
		{
			m_bindings.emplace_back( std::make_pair( e, Core::Intent(intentID, Core::Intent::Type::Range) ) );
		}
	}

	void Context::addState(uint32_t intentID, const Event& e)
	{
		if(e.type != EventCode::Axis)
		{
			m_bindings.emplace_back( std::make_pair( e, Core::Intent(intentID, Core::Intent::Type::State) ) );
		}
	}

	void Context::addRangeConverter(uint32_t intentID, const Converter& c)
	{
		m_converters.emplace(intentID, c);
	}

	void Context::addRangeConverter(uint32_t intentID, double minIn, double maxIn, double minOut, double maxOut)
	{
		m_converters.emplace(intentID, Converter(minIn, maxIn, minOut, maxOut));
	}
}