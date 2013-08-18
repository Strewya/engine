//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Input/MappedInput.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace SG
{
	void MappedInput::AddAction(const String action)
	{
		_actions.insert(action);
	}

	void MappedInput::AddState(const String state)
	{
		_states.insert(state);
	}

	void MappedInput::AddRange(const String range, double value)
	{
		_ranges[range] = value;
	}

	void MappedInput::EatAction(const String& action)
	{
		_actions.erase(action);
	}
    
    void MappedInput::EatState(const String& state)
	{
		_states.erase(state);
	}

	void MappedInput::EatRange(const String& range)
	{
		std::map<String, double>::iterator iter = _ranges.find(range);
		if(iter != _ranges.end())
			_ranges.erase(iter);
	}

	bool MappedInput::HasAction(const String& action)
	{
		return _actions.find(action) != _actions.end();
	}

	bool MappedInput::HasState(const String& state)
	{
		return _states.find(state) != _states.end();
	}

	bool MappedInput::HasRange(const String& range)
	{
		return _ranges.find(range) != _ranges.end();
	}

	double MappedInput::RangeValue(const String& range)
	{
		if(HasRange(range))
			return _ranges[range];
		return 0.0;
	}

	bool MappedInput::IsEmpty()
	{
		return (_actions.empty() && _states.empty() && _ranges.empty());
	}

	void MappedInput::ClearActions() { _actions.clear(); }
	void MappedInput::ClearRanges() { _ranges.clear(); }
}