#pragma once
/********************************************
*	class:	Debug utilities
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <iostream>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{

#define CLASS(x) "Core::"#x

#define DEBUG_STATUS(x) bool status = x
#define DEBUG_INIT(c) DEBUG_INFO( #c" init ", status ? "OK" : "FAIL"); return status
#define DEBUG_SHUTDOWN(c) DEBUG_INFO( #c" shutdown ", status ? "OK" : "FAIL"); return status


#ifndef DEPLOY

	inline void debugPrint() { std::cout << std::endl; }
	template<typename T, typename... Args> void debugPrint(T t, Args... args) { std::cout << t; debugPrint(args...); }

#define DEBUG_INFO(...) debugPrint(__VA_ARGS__)

#else
#define DEBUG_INFO(...) (void)0
#endif

#define Deg2Rad(deg) ((deg)*3.14159265358979f/180.0f)
#define Rad2Deg(rad) ((rad)*180.0f/3.14159265358979f)


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
