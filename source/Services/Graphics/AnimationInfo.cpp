//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Graphics/AnimationInfo.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	AnimationInfo::AnimationInfo()
		: _name()
	{
	}

	AnimationInfo::~AnimationInfo()
	{
	}

	void AnimationInfo::setName(const std::string& name)
	{
		_name = name;
	}

	const std::string& AnimationInfo::getName() const
	{
		return _name;
	}
	
	void AnimationInfo::AddToSequence(int id)
	{
		_sequence.push_back(id);
	}

	uint32_t AnimationInfo::getSequenceFrame(uint32_t index) const
	{
		return _sequence[index];
	}

	size_t AnimationInfo::getSequenceSize() const
	{
		return _sequence.size();
	}

	const std::deque<int>& AnimationInfo::getSequence() const
	{
		return _sequence;
	}

	void AnimationInfo::ClearSequence()
	{
		_sequence.clear();
	}
}