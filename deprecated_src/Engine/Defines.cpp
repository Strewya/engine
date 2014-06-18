//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include <Engine/Defines.h>
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

std::string trim(const std::string& str, const char* t)
{
	std::string s = str;
	s.erase(0, s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t)+1);
	return s;
}

std::string lowercase(const std::string& s)
{
	std::string ret;
	ret.resize(s.length());
	std::transform(s.begin(), s.end(), ret.begin(), tolower);
	return ret;
}

std::string uppercase(const std::string& s)
{
	std::string ret;
	ret.resize(s.length());
	std::transform(s.begin(), s.end(), ret.begin(), toupper);
	return ret;
}
