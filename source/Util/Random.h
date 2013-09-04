#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include <random>
	/*** end header inclusion ***/
	
namespace Util
{
	class Random
	{
	public:
		Random();
		Random(uint32_t seed);

		uint32_t rand(uint32_t lowerBound, uint32_t upperBound);
		
	private:
		std::mt19937 m_randEngine;
	};
}