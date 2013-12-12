//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Modules/Rendering/Service/D3D11/IndexBuffer.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/
	
namespace Graphics
{
	IndexBuffer::IndexBuffer()
		: id(0), size(0), buffer(nullptr), dev(nullptr), devcon(nullptr)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
		buffer->Release();
	}

	void IndexBuffer::copyData(const std::vector<uint32_t>& data)
	{
		assert(buffer != nullptr);
		size = data.size();
		
		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, data.data(), data.size()*sizeof(uint32_t));
		devcon->Unmap(buffer, 0);
	}

	void IndexBuffer::bind()
	{
		devcon->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::draw()
	{
		devcon->DrawIndexed(size, 0, 0);
	}
}