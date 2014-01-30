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
#define RESOURCE_S(name) ("../resources/" + name).c_str()

#define AS_STRING(x) #x

#ifdef _DEBUG

#define DEBUG_INFO(...) debugPrint(__VA_ARGS__)
#define DEBUG_LINE(func) func

	inline void debugPrint() { std::cout << std::endl; }
	template<typename T, typename... Args> void debugPrint(T t, Args... args) { std::cout << t; debugPrint(args...); }

#else
#define DEBUG_INFO(...) void(0)
#define DEBUG_LINE(func) void(0)
#endif
}
