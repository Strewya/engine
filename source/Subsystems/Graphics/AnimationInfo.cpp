//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/AnimationInfo.h"
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

	void AnimationInfo::setName(const String& name)
	{
		_name = name;
	}

	const String& AnimationInfo::getName() const
	{
		return _name;
	}
	
	void AnimationInfo::AddToSequence(int id)
	{
		_sequence.push_back(id);
	}

	int AnimationInfo::getSequenceFrame(uint index) const
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