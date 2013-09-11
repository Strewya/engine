	/*** precompiled header ***/
#include "stdafx.h"

#include "dll_header.h"
#include "Services/Graphics/IRenderer.h"
#include "Services/Graphics/D3D11/DXRenderer.h"
#include "Util/Logger.h"

EXPORT int createRendererInterface(HWND hwnd, uint32_t screenW, uint32_t screenH, Graphics::IRenderer** renderer)
{
	if(renderer != nullptr && *renderer == nullptr)
	{
		try
		{
			*renderer = new Graphics::DXRenderer(hwnd, screenW, screenH);
			return 1;
		}
		catch(std::exception& ex)
		{
			Util::GetDefaultLogger() << ex.what() << Util::Logger::endl;
		}
	}
	return 0;
}

EXPORT int destroyRendererInterface(Graphics::IRenderer* renderer)
{
	if(renderer != nullptr)
	{
		delete renderer;
	}
	return 1;
}
