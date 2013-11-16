#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
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
		Color(float r, float g, float b, float a = 1.0f);
		Color(const float* RGBA);
		~Color();

		Color& operator=(const Color& c);
		bool operator==(const Color& rhs) const;
		Color& set(const Color& c);

		void setRed(float value);
		void setGreen(float value);
		void setBlue(float value);
		void setAlpha(float value);
		void setRGBA(float r, float g, float b, float a = 1.0f);

		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;
		const float *const getRGBA() const;

	private:
		float r, g, b, a;
	};
}

std::istream& operator>>(std::istream& ss, Util::Color& v);
std::ostream& operator<<(std::ostream& ss, Util::Color& v);