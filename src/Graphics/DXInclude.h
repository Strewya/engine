#pragma once

#include <d3d11.h>				//directx
#include <d3dx11.h>				//directx
#include <d3dx10.h>				//directx
#include <xnamath.h>
#include <dxgi.h>

template<typename T> void releasePtr(T* ptr)
{
   if( ptr != nullptr )
   {
      ptr->Release();
   }
}

template<typename T> void safeRelease(T*& ptr)
{
   if( ptr != nullptr )
   {
      releasePtr(ptr);
      ptr = nullptr;
   }
}
