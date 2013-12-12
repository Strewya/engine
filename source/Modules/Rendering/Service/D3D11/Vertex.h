#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <vector>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Modules/Rendering/Service/D3D11/DXInclude.h>
	/*** end header inclusion ***/

namespace Graphics
{
	struct Vertex
	{
		Vertex() {};
		Vertex(float x, float y, float z, float r, float g, float b, float a, float u, float v) : pos(x,y,z), diffuse(r,g,b,a), texCoord(u,v) {}
		void setPosition(float x, float y, float z);
		void setDiffuse(float r, float g, float b, float a);
		void setTextureCoords(float u, float v);

		XMFLOAT3 pos;
		XMFLOAT4 diffuse;
		XMFLOAT2 texCoord;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> getDescription();
	};

	
}