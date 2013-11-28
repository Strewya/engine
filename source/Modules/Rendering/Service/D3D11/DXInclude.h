#pragma once

#ifdef _DEBUG
#define _XM_NO_INTRINSICS_
#endif

#include <d3d11.h>				//directx
#include <d3dx11.h>				//directx
#include <d3dx10.h>				//directx
#include <xnamath.h>

#include "Util/Color.h"
#include "Util/Dimensional.h"
#include "Services/Graphics/Graphics.h"

namespace Graphics
{
	inline RECT MakeRECT(const Util::Rect& in)
	{
		RECT out = {(int)in.Left(), (int)in.Top(), (int)in.Right(), (int)in.Bottom()} ;
		return out;
	}

	inline XMFLOAT2 MakeVECTOR(const Util::Vec2& in)
	{
		return XMFLOAT2(in.x, in.y);
	}

	inline XMFLOAT3 MakeVECTOR(const Util::Vec3& in)
	{
		return XMFLOAT3(in.x, in.y, in.z);
	}

	struct cbPerObject
	{
		XMMATRIX WVP;
	};
}