#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Services/Graphics/IIndexBuffer.h"
#include "Services/Graphics/D3D11/DXInclude.h"
	/*** end header inclusion ***/
	
namespace Graphics
{
	class IRenderer;

	class IndexBuffer : public IIndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();
		void copyData(const std::vector<uint32_t>& data);
		void bind();
		void draw();

		uint32_t id;
		uint32_t size;
		ID3D11Buffer* buffer;
		ID3D11Device* dev;
		ID3D11DeviceContext* devcon;
	};
}