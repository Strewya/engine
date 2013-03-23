#ifndef SUBSYSTEMS_GRAPHICS_DLL_HEADER_H_
#define SUBSYSTEMS_GRAPHICS_DLL_HEADER_H_

#include <windows.h>

namespace Graphics { class Interface; }

int createRendererInterface(HWND hwnd, Graphics::Interface** renderer);
int destroyRendererInterface(Graphics::Interface** renderer);

typedef int(*CREATE_RENDERER)(HWND hwnd, Graphics::Interface** renderer);
typedef int(*DESTROY_RENDERER)(Graphics::Interface** renderer);

#endif //SUBSYSTEMS_GRAPHICS_DLL_HEADER_H_