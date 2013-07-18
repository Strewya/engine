#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Color.h"
	/*** end header inclusion ***/

namespace Graphics
{
	struct Vertex
	{
		float x,y,z;
		DWORD diffuse;
	};
}