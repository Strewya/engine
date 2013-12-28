#pragma once

#ifdef DLL_BUILD
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C" __declspec(dllimport)
#endif

#include <windows.h>

namespace Graphics { class IRenderer; }
namespace Win32 { class Window; }

EXPORT int getRendererInterface(Win32::Window* window,  Graphics::IRenderer** renderer);

typedef int(*GET_RENDERER)(Win32::Window* window, Graphics::IRenderer** renderer);
