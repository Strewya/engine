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
		uint32_t m_textureID;
		float m_ratio;
		Vec2 m_texCoords[4];
		std::string m_name;
	};
}
