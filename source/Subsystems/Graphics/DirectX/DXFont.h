#ifndef SUBSYSTEMS_GRAPHICS_DXFONT_H_
#define SUBSYSTEMS_GRAPHICS_DXFONT_H_
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
		DXFont(const char* name, uint size, bool italic, uint weight, LPD3DXFONT f);
		DXFont(const String& name, uint size, bool italic, uint weight, LPD3DXFONT f);

		FontInfo info;
		LPD3DXFONT data;
		
		void Release();
	};
}

#endif //SUBSYSTEMS_GRAPHICS_DXFONT_H_