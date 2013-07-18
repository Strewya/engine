#pragma once

#include <d3d9.h>				//directx
#include <d3dx9.h>				//directx
#include <d3dx9math.h>			//directx
#include "Util/Color.h"
#include "Util/Dimensional.h"

inline RECT MakeRECT(const Util::Rect& in)
{
	RECT out = {(int)in.Left(), (int)in.Top(), (int)in.Right(), (int)in.Bottom()} ;
	return out;
}

inline D3DXVECTOR2 MakeVECTOR(const Util::Vec2& in)
{
	return D3DXVECTOR2(in.x, in.y);
}

inline D3DXVECTOR3 MakeVECTOR(const Util::Vec3& in)
{
	return D3DXVECTOR3(in.x, in.y, in.z);
}