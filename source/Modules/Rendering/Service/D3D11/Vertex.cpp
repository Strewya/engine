//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Graphics/D3D11/Vertex.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Graphics
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> Vertex::getDescription()
	{
		D3D11_INPUT_ELEMENT_DESC data[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc(data, data+3);
		return desc;
	}

	void Vertex::setPosition(float x, float y, float z)
	{
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	void Vertex::setDiffuse(float r, float g, float b, float a)
	{
		diffuse.x = r;
		diffuse.y = g;
		diffuse.z = b;
		diffuse.w = a;
	}

	void Vertex::setTextureCoords(float u, float v)
	{
		texCoord.x = u;
		texCoord.y = v;
	}
}