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
		Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(uint32_t colorARGB);
		~Color();

		Color& operator=(const Color& c);
		bool operator==(const Color& rhs) const;
		Color& Assign(const Color& c);

		void setRed(uint8_t value);
		void setGreen(uint8_t value);
		void setBlue(uint8_t value);
		void setAlpha(uint8_t value);
		void setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

		uint8_t getRed() const;
		uint8_t getGreen() const;
		uint8_t getBlue() const;
		uint8_t getAlpha() const;
		uint32_t getARGB() const;

	private:
		uint32_t _colorARGB;
	};
}

std::istream& operator>>(std::istream& ss, Util::Color& v);
std::ostream& operator<<(std::ostream& ss, Util::Color& v);