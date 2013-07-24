//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXFont.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	DXFont::DXFont()
		: info(), data(nullptr)
	{}

	DXFont::DXFont(const char* name, uint32_t size, bool italic, uint32_t weight, LPD3DXFONT f)
		: info(name, size, italic, weight), data(f)
	{}

	DXFont::DXFont(const String& name, uint32_t size, bool italic, uint32_t weight, LPD3DXFONT f)
		: info(name, size, italic, weight), data(f)
	{}

	void DXFont::Release()
	{
		if(data)
		{
			data->Release();
			data = nullptr;
		}
	}
}