#pragma once

/*		C++ HEADERS	*/
#include <cassert>
#include <stdint.h>
#include <string>
#include <sstream>

/*		TYPEDEFS	*/
typedef uint32_t InstanceID;
typedef unsigned long DWORD;

/*		CONTANTS	*/
const double PI_D = 3.141592653589793238462;
const double DEG2RAD_RATIO_D = PI_D/180.0;
const double RAD2DEG_RATIO_D = 180.0/PI_D;

const float  PI_F = 3.14159265358979f;
const float  DEG2RAD_RATIO_F = PI_F/180.0f;
const float  RAD2DEG_RATIO_F = 180.0f/PI_F;

enum ErrorCode
{
	OK = 0,
	WindowClassRegistration,
	WindowCreation,
	WindowNotExists,
	ExceptionThrown
};

/*		MACROS		*/
#define NO_ID 0
#define NOT_FOUND -1
#define INVALID_ID -1

/*		FUNCTIONS	*/
float  Deg2Rad(float angle);
double Deg2Rad(double angle);
float  Rad2Deg(float radians);
double Rad2Deg(double radians);
std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");
std::string lowercase(const std::string& s);
std::string uppercase(const std::string& s);

template <typename T> std::string toString(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template<typename T, typename L, typename H> T clamp(const T& value, const L& low, const H& high)
{
	if(value < low)
		return low;
	
	if(value > high)
		return high;
	
	return value;
}

/*		MY HEADERS	*/
#include <Util/Logger.h>
#include <Engine/FwdDecl.h>
