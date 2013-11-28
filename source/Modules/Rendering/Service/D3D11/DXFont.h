#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXInclude.h"
#include "Subsystems/Graphics/FontInfo.h"
	/*** end header inclusion ***/

namespace Graphics
{
    class DXFont
	{
	public:
		DXFont();
		DXFont(const char* name, uint32_t size, bool italic, uint32_t weight, LPD3DXFONT f);
		DXFont(const String& name, uint32_t size, bool italic, uint32_t weight, LPD3DXFONT f);

		FontInfo info;
		LPD3DXFONT data;
		
		void Release();
	};
}