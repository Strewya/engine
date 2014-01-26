#pragma once
/********************************************
*	class:	Color
*	usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	class Color
	{
	public:
		float r, g, b, a;

		Color();
		Color(float r, float g, float b);

		void set(float r, float g, float b);
	};
}

	/*class Color
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
        float const * const getRGBA() const;

	private:
		float r, g, b, a;
	};*/
