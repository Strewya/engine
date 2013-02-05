//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Defines.h"
	/*** C++ headers ***/
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