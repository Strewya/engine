
	/*** personal header ***/
#include "SGInputContext.h"
	/*** extra headers ***/
	/*** end headers ***/

namespace SG
{
	
	InputContext::InputContext(ticpp::Element* xml)
		: _next(NULL), _name(), _propagates()
	{
		//do xml parsing for this context
		_name = xml->GetAttribute("name");
		xml->GetAttributeOrDefault("propagates", &_propagates, true);

		String type, name, input;
		//read the bindings
		ticpp::Iterator< ticpp::Element > it;
		for(it = it.begin(xml); it != it.end(); ++it)
		{
			type = it->GetAttribute("type");
			if(type == "action")
			{
				it->GetAttribute("key", &input);
				it->GetAttribute("event", &name);
				_actions.insert( std::make_pair( Keys::Code(input), name ) );
			}
			else if(type == "state")
			{
				it->GetAttribute("key", &input);
				it->GetAttribute("event", &name);
				_states.insert( std::make_pair( Keys::Code(input), name ) );
			}
			else if(type == "range")
			{
				it->GetAttribute("axis", &input);
				it->GetAttribute("event", &name);
				_ranges.insert( std::make_pair( Axis::Code(input), name ) );
			}
		}
	}

	InputContext* InputContext::Push(InputContext* context)
	{
		context->_next = this;

		return context;
	}

	InputContext* InputContext::Pop()
	{
		InputContext* ret = _next == NULL ? this : _next;
		_next = NULL;
		return ret;
	}

	bool InputContext::MapButtonToState(uint button, String& out) const
	{
		Mapping::const_iterator it = _states.find(button);
		if(it == _states.end())
		{
			if(_next != NULL && _propagates)
			{
				return _next->MapButtonToState(button, out);
			}
			return false;
		}

		out = it->second;
		return true;
	}

	bool InputContext::MapButtonToAction(uint button, String& out) const
	{
		Mapping::const_iterator it = _actions.find(button);
		if(it == _actions.end())
		{
			if(_next != NULL && _propagates)
			{
				return _next->MapButtonToAction(button, out);
			}
			return false;
		}

		out = it->second;
		return true;
	}

	bool InputContext::MapAxisToRange(uint axis, String& out) const
	{
		Mapping::const_iterator it = _ranges.find(axis);
		if(it == _ranges.end())
		{
			if(_next != NULL && _propagates)
			{
				return _next->MapAxisToRange(axis, out);
			}
			return false;
		}

		out = it->second;
		return true;
	}

	String InputContext::getName() const
	{
		return _name;
	}

	bool InputContext::isPropagate() const
	{
		return _propagates;
	}

	void InputContext::setPropagation(bool p)
	{
		_propagates = p;
	}
}