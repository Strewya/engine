//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/window_class.h"
/******* c++ headers *******/
#include <cstring>
/******* extra headers *******/
#include "window/window.h"
/******* end headers *******/

namespace Core
{
   WindowClass::WindowClass(const char* className)
      : WindowClass(::GetModuleHandle(nullptr), className)
   {
   }

   WindowClass::WindowClass(const std::string& className)
      : WindowClass(className.c_str())
   {
   }

   WindowClass::WindowClass(HINSTANCE hInst, const std::string& className)
      : WindowClass(hInst, className.c_str())
   {
   }

   WindowClass::WindowClass(HINSTANCE hInst, const char* className)
   {
      FillDefaultData();
      this->hInstance = hInst;
      this->lpszClassName = TEXT(className);
   }

   void WindowClass::FillDefaultData()
   {
      this->cbClsExtra = 0;
      this->cbSize = sizeof(WNDCLASSEX);
      this->cbWndExtra = 0;
      this->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
      this->hCursor = LoadCursor(nullptr, IDC_ARROW);
      this->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
      this->hIconSm = nullptr;
      this->hInstance = nullptr; //this is reset in ctor
      this->lpfnWndProc = (WNDPROC)Window::messageRouter;
      this->lpszClassName = nullptr; //this is reset in ctor
      this->lpszMenuName = nullptr;
      this->style = CS_HREDRAW | CS_VREDRAW;
   }

   ATOM WindowClass::registerClass()
   {
      return RegisterClassEx(this);
   }

   const TCHAR* WindowClass::getClassName() const
   {
      return this->lpszClassName;
   }
}