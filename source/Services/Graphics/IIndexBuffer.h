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
	class IIndexBuffer
	{
	public:
		virtual void copyData(const std::vector<uint32_t>& data) = 0;
		virtual void bind() = 0;
		virtual void draw() = 0;
	};
}