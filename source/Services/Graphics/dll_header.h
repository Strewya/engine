#pragma once

#ifdef DLL_BUILD
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C" __declspec(dllimport)
#endif

#include <windows.h>

namespace Graphics { class IRenderer; }

EXPORT int createRendererInterface(HWND hwnd, Graphics::IRenderer** renderer);
EXPORT int destroyRendererInterface(Graphics::IRenderer* renderer);

typedef int(*CREATE_RENDERER)(HWND hwnd, Graphics::IRenderer** renderer);
typedef int(*DESTROY_RENDERER)(Graphics::IRenderer* renderer);
