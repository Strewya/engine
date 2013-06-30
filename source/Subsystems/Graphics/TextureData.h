#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Graphics
{
	class TextureData
	{
	public:
		TextureData();
		
		uint32_t width;
		uint32_t height;
	};
}