#pragma once

#ifdef DLL_BUILD
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C" __declspec(dllimport)
#endif

#include <windows.h>

namespace Graphics { class IRenderer; }

EXPORT int createRendererInterface(HWND hwnd, uint32_t screenW, uint32_t screenH,  Graphics::IRenderer** renderer);
EXPORT int destroyRendererInterface(Graphics::IRenderer* renderer);

typedef int(*CREATE_RENDERER)(HWND hwnd, uint32_t screenW, uint32_t screenH, Graphics::IRenderer** renderer);
typedef int(*DESTROY_RENDERER)(Graphics::IRenderer* renderer);
