#pragma once
/********************************************
*	class:	Animation
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Animation
	{
	public:
		uint32_t m_defaultDuration;
		uint32_t m_spritesheetID;
		std::vector<uint32_t> m_sequence;
		std::string m_name;
		bool m_defaultRepeat;
	};
}
