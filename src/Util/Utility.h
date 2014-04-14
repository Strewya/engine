#pragma once
/********************************************
*	class:	Debug utilities
*	usage:
********************************************/
/******* C++ headers *******/
#include <iostream>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
#define RESOURCE(name) "../resources/"name
#define RESOURCE_S(name) std::string("../resources/").append(name).c_str()

#define CLASS(x) "Core::"#x

#define DEBUG_INIT(c) DEBUG_INFO( #c" init ", status ? "OK" : "FAIL")
#define DEBUG_SHUTDOWN(c) DEBUG_INFO( #c" shutdown ", status ? "OK" : "FAIL")

#ifdef _DEBUG

#define DEBUG_INFO(...) debugPrint(__VA_ARGS__)
#define DEBUG_CODE_START
#define DEBUG_CODE_END

	inline void debugPrint() { std::cout << std::endl; }
	template<typename T, typename... Args> void debugPrint(T t, Args... args) { std::cout << t; debugPrint(args...); }

#else
#define DEBUG_INFO(...) void(0)
#define DEBUG_CODE_START if(false) {
#define DEBUG_CODE_END }
#endif


	template<typename T> T& wrap(const T& low, const T& high, T& value)
	{
		auto d = high - low;
		if(value < low)
		{
			value += d;
		}
		else if(value > high)
		{
			value -= d;
		}
		return value;
	}

	template<typename T> T& clamp(const T& low, const T& high, T& value)
	{
		if(value < low)
		{
			value = low;
		}
		else if(value > high)
		{
			value = high;
		}
		return value;
	}

}
