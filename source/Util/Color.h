#ifndef UTIL_COLOR_H_
#define UTIL_COLOR_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <iostream>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	class Color
	{
	public:
		Color();
		Color(const Color &c);
		Color(uint r, uint g, uint b, uint a=255);
		~Color();

		Color& operator=(const Color& c);
		bool operator==(const Color& rhs);
		Color& Assign(const Color& c);

		void setRed(uint value);
		void setGreen(uint value);
		void setBlue(uint value);
		void setAlpha(uint value);
		void setChannels(uint r, uint g, uint b, uint a = 255);

		uint const& red;
		uint const& green;
		uint const& blue;
		uint const& alpha;


	private:
		uint _red;
		uint _green;
		uint _blue;
		uint _alpha;
	};
}

std::istream& operator>>(std::istream& ss, Util::Color& v);
std::ostream& operator<<(std::ostream& ss, Util::Color& v);

#endif //UTIL_COLOR_H_