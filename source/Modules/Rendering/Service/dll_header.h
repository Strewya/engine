#pragma once

#ifdef DLL_BUILD
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C" __declspec(dllimport)
#endif

#include <windows.h>

namespace Graphics { class IRenderer; }

EXPORT int getRendererInterface(HWND hwnd, uint32_t screenW, uint32_t screenH,  Graphics::IRenderer** renderer);

typedef int(*GET_RENDERER)(HWND hwnd, uint32_t screenW, uint32_t screenH, Graphics::IRenderer** renderer);
