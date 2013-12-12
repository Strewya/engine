	/*** precompiled header ***/
#include <stdafx.h>

#include <Modules/Rendering/Service/dll_header.h>
#include <Modules/Rendering/Service/IRenderer.h>
#include <Modules/Rendering/Service/D3D11/DXRenderer.h>
#include <Util/Logger.h>

EXPORT int getRendererInterface(HWND hwnd, uint32_t screenW, uint32_t screenH, Graphics::IRenderer** renderer)
{
	if(renderer != nullptr)
	{
		try
		{
			static Graphics::DXRenderer dxRenderer(hwnd, screenW, screenH);
			*renderer = &dxRenderer;
			return 1;
		}
		catch(std::exception& ex)
		{
			Util::GetDefaultLogger() << ex.what() << Util::Logger::endl;
		}
	}
	return 0;
}