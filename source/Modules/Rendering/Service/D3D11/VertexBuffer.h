#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Modules/Rendering/Service/IVertexBuffer.h"
#include "Modules/Rendering/Service/D3D11/DXInclude.h"
	/*** end header inclusion ***/
	
namespace Graphics
{
	class DXRenderer;

	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();
		void copyData(const std::vector<Util::Vec2>& position, const Util::Color& color);
		void setTopology(Topology t);
		void bind();
		void draw();

		uint32_t id;
		uint32_t size;
		ID3D11Buffer* buffer;
		ID3D11DeviceContext* devcon;
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}
