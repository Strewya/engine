#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXInclude.h"
	/*** end header inclusion ***/

namespace Graphics
{
	struct VERTEX
	{
		VERTEX() {};
		VERTEX(float x, float y, float z) : pos(x,y,z), diffuse(1.0f,1.0f,1.0f,1.0f) {}
		VERTEX(float x, float y, float z, float r, float g, float b, float a = 1.0f) : pos(x,y,z), diffuse(r,g,b,a) {}

		XMFLOAT3 pos;
		XMFLOAT4 diffuse;
	};
}