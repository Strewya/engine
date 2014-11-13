#pragma once
/********************************************
*	class:	Random
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <random>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Random
	{
	public:
		Random();
		Random(uint32_t seed);

		//change the seed
		void reseed(uint32_t seed);

		//in interval between bounds
		int32_t randInt(int32_t lowerBound, int32_t upperBound);

		//in interval between 0 and 1
		float randFloat();

	private:
		std::mt19937 m_randEngine;
	};
}