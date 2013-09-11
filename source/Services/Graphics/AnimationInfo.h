#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
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

		void setName(const std::string& name);
		const std::string& getName() const;
		/*
		void setRepeatMode(RepeatMode mode);
		RepeatMode getRepeatMode() const;
		*/
		void AddToSequence(int id);
		uint32_t getSequenceFrame(uint32_t index) const;
		size_t getSequenceSize() const;
		const std::deque<int>& getSequence() const;


		void ClearSequence();

	private:
		std::string _name;
		std::deque<int> _sequence;
	};
}