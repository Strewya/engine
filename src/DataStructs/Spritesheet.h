#pragma once
/********************************************
*	class:	Spritesheet
*	usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <DataStructs/Image.h>
/******* end header inclusion *******/

namespace Core
{
	class Spritesheet
	{
	public:
		uint32_t m_textureID;
		std::string m_name;
		std::vector<Image> m_images;
	};
}
