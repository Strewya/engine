#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Graphics/DirectX/DXInclude.h"
	/*** end header inclusion ***/

namespace Graphics
{
	struct vertex_pos
	{
		vertex_pos() {};
		vertex_pos(float x, float y, float z) : pos(x,y,z) {}

		XMFLOAT3 pos;
	};

	D3D11_INPUT_ELEMENT_DESC vertex_pos_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	struct vertex_pos_diffuse
	{
		vertex_pos_diffuse() {};
		vertex_pos_diffuse(float x, float y, float z) : pos(x,y,z), diffuse(1.0f,1.0f,1.0f,1.0f) {}
		vertex_pos_diffuse(float x, float y, float z, float r, float g, float b, float a = 1.0f) : pos(x,y,z), diffuse(r,g,b,a) {}

		XMFLOAT3 pos;
		XMFLOAT4 diffuse;
	};

	D3D11_INPUT_ELEMENT_DESC vertex_pos_diffuse_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	struct vertex_pos_texture
	{
		vertex_pos_texture() {};
		vertex_pos_texture(float x, float y, float z) : pos(x,y,z), texCoord(0.0f,0.0f) {}
		vertex_pos_texture(float x, float y, float z, float u, float v) : pos(x,y,z), texCoord(u,v) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
	};

	D3D11_INPUT_ELEMENT_DESC vertex_pos_texture_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};




#define VERTEX vertex_pos_diffuse
#define VERTEX_DESC vertex_pos_diffuse_desc
}