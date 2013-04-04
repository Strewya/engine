//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Engine/Defines.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
	/*** end headers ***/
	
float Deg2Rad(float deg)
{
	return deg * DEG2RAD_RATIO_F;
}

float Rad2Deg(float rad)
{
	return rad * RAD2DEG_RATIO_F;
}

double Deg2Rad(double deg)
{
	return deg * DEG2RAD_RATIO_D;
}

double Rad2Deg(double rad)
{
	return rad * RAD2DEG_RATIO_D;
}

String trim(const String& str, const char* t)
{
	String s = str;
	s.erase(0, s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t)+1);
	return s;
}

String lowercase(const String& s)
{
	String ret;
	ret.resize(s.length());
	std::transform(s.begin(), s.end(), ret.begin(), tolower);
	return ret;
}

String uppercase(const String& s)
{
	String ret;
	ret.resize(s.length());
	std::transform(s.begin(), s.end(), ret.begin(), toupper);
	return ret;
}