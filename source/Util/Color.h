#pragma once
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
		Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a=255);
		~Color();

		Color& operator=(const Color& c);
		bool operator==(const Color& rhs);
		Color& Assign(const Color& c);

		void setRed(uint32_t value);
		void setGreen(uint32_t value);
		void setBlue(uint32_t value);
		void setAlpha(uint32_t value);
		void setChannels(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);

		uint32_t const& red;
		uint32_t const& green;
		uint32_t const& blue;
		uint32_t const& alpha;


	private:
		uint32_t _red;
		uint32_t _green;
		uint32_t _blue;
		uint32_t _alpha;
	};
}

std::istream& operator>>(std::istream& ss, Util::Color& v);
std::ostream& operator<<(std::ostream& ss, Util::Color& v);