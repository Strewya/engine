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
		FLOAT X, Y, Z;
		D3DXCOLOR COLOR;
	};
}