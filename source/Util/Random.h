#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
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

		//in itnerval between bounds
		int32_t randInt(int32_t lowerBound, int32_t upperBound);
		//in interval between 0 and 1
		float randFloat();
		
	private:
		std::mt19937 m_randEngine;
	};
}