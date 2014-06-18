//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Modules/Graphics/Service/D3D11/VertexBuffer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Modules/Graphics/Service/D3D11/DXRenderer.h"
#include "Modules/Graphics/Service/D3D11/Vertex.h"
	/*** end headers ***/
	
namespace Graphics
{
	VertexBuffer::VertexBuffer()
		: id(0), size(0), buffer(nullptr), devcon(nullptr), topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	{}

	VertexBuffer::~VertexBuffer()
	{
		HRESULT hr = buffer->Release();
	}

	void VertexBuffer::setTopology(Topology t)
	{
		static D3D11_PRIMITIVE_TOPOLOGY d3dTopologies[] = 
		{
			D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		};

		topology = d3dTopologies[(uint32_t)t];
	}

	void VertexBuffer::copyData(const std::vector<Util::Vec2>& position, const Util::Color& color)
	{
		assert(buffer != nullptr);
		size = position.size();
		std::vector<Vertex> vertices(size);
		for(uint32_t i = 0; i < size; ++i)
		{
			vertices[i].pos = XMFLOAT3(position[i].x, position[i].y, 0.5f);
			vertices[i].diffuse = XMFLOAT4(color.getRGBA());
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), vertices.size()*sizeof(Vertex));
		devcon->Unmap(buffer, 0);
	}

	void VertexBuffer::bind()
	{
		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		devcon->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
		devcon->IASetPrimitiveTopology(topology);
	}

	void VertexBuffer::draw()
	{
		devcon->Draw(size, 0);
	}
}
