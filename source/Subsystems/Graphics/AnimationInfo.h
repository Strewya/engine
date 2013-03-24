#ifndef SUBSYSTEMS_GRAPHICS_ANIMATION_H_
#define SUBSYSTEMS_GRAPHICS_ANIMATION_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class AnimationInfo
	{
	public:
		/*
		enum RepeatMode
		{
			None,
			Loop
		};
		*/
		AnimationInfo();
		~AnimationInfo();

		void setName(const String& name);
		const String& getName() const;
		/*
		void setRepeatMode(RepeatMode mode);
		RepeatMode getRepeatMode() const;
		*/
		void AddToSequence(int id);
		int getSequenceFrame(uint index) const;
		size_t getSequenceSize() const;
		const std::deque<int>& getSequence() const;


		void ClearSequence();

	private:
		String _name;
		std::deque<int> _sequence;
	};
}

#endif //SUBSYSTEMS_GRAPHICS_ANIMATION_H_