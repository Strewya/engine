#pragma once
/********************************************
*	class:	Texture
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <string>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Texture
	{
	public:
		std::string m_name;
		uint32_t m_index;
	};
}
