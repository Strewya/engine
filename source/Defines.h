#ifndef DEFINES_H_
#define DEFINES_H_

/*		C++ HEADERS	*/
#include <cassert>
#include <string>
#include <sstream>

/*		TYPEDEFS	*/
typedef std::string String;
typedef unsigned int uint;
typedef long InstanceID;
typedef unsigned long DWORD;
typedef unsigned long ulong;

/*		CONTANTS	*/
const uint gDefaultDataStoreSize = 64;
const double PI_D = 3.141592653589793238462;
const double DEG2RAD_RATIO_D = PI_D/180.0;
const double RAD2DEG_RATIO_D = 180.0/PI_D;

const float  PI_F = 3.14159265358979f;
const float  DEG2RAD_RATIO_F = PI_F/180.0f;
const float  RAD2DEG_RATIO_F = 180.0f/PI_F;

enum ErrorCode
{
	WindowClassRegistration = 1,
	WindowCreation,
	WindowNotExists,
	ExceptionThrown
};

/*		MACROS		*/
#define NO_ID 0
#define NOT_FOUND -1

/*		FUNCTIONS	*/
float  Deg2Rad(float angle);
double Deg2Rad(double angle);
float  Rad2Deg(float radians);
double Rad2Deg(double radians);
template <typename T> String ToString(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

/*		MY HEADERS	*/
#include "Util/Clock.h"
#include "Util/Logger.h"

#endif //DEFINES_H_