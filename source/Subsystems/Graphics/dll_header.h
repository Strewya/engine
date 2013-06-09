#pragma once

#include <windows.h>

namespace Graphics { class IRenderer; }

int createRendererInterface(HWND hwnd, Graphics::IRenderer** renderer);
int destroyRendererInterface(Graphics::IRenderer** renderer);

typedef int(*CREATE_RENDERER)(HWND hwnd, Graphics::IRenderer** renderer);
typedef int(*DESTROY_RENDERER)(Graphics::IRenderer** renderer);