
#include "dll_header.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/DirectX/DXRenderer.h"

int createRendererInterface(HWND hwnd, Graphics::IRenderer** renderer)
{
	if(!*renderer)
	{
		try
		{
			*renderer = new Graphics::DXRenderer(hwnd);
			return 1;
		}
		catch(std::exception& ex)
		{
			ex;
		}
	}
	return 0;
}

int destroyRendererInterface(Graphics::IRenderer** renderer)
{
	if(!*renderer)
	{
		return 0;
	}
	delete *renderer;
	*renderer = NULL;
	return 1;
}