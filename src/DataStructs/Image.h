#pragma once
/********************************************
*	class:	Image
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Vec2.h>
/******* end header inclusion *******/

namespace Core
{
	class Image
	{
	public:
		Vec2 m_texCoords[4];
		float m_ratio;
		std::string m_name;
	};
}
