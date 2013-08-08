	/*** precompiled header ***/
#include "stdafx.h"

#include "dll_header.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/DirectX/DXRenderer.h"
#include "Util/Logger.h"

EXPORT int createRendererInterface(HWND hwnd, Graphics::IRenderer** renderer)
{
	if(renderer != nullptr && *renderer == nullptr)
	{
		try
		{
			*renderer = new Graphics::DXRenderer(hwnd);
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
	if(renderer == nullptr)
	{
		return 0;
	}
	delete renderer;
	return 1;
}
