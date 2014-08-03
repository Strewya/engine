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
		if( value < low )
		{
			value += d;
		}
		else if( value > high )
		{
			value -= d;
		}
		return value;
	}

	template<typename T> T& clamp(const T& low, const T& high, T& value)
	{
		if( value < low )
		{
			value = low;
		}
		else if( value > high )
		{
			value = high;
		}
		return value;
	}

	inline std::string& replace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if( start_pos != std::string::npos )
		{
			str.replace(start_pos, from.length(), to);
		}
		return str;
	}

	template<typename C, typename F> size_t filterFind(const C& container, const F& filter)
	{
		return std::distance(std::begin(container), std::find_if(std::begin(container), std::end(container), filter));
	}

	template<typename C, typename T> size_t valueFind(const C& container, const T& value)
	{
		return std::distance(std::begin(container), std::find(std::begin(container), std::end(container), value));
	}
}
