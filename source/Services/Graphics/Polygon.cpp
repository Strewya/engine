//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include "stdafx.h"
	/*** personal header ***/
#include "Services/Graphics/Polygon.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Services/Graphics/Graphics.h"
#include "Services/Graphics/IRenderer.h"
#include "Services/Graphics/IIndexBuffer.h"
#include "Services/Graphics/IVertexBuffer.h"
#include "Util/Color.h"
	/*** end headers ***/
	
namespace Graphics
{
	void Polygon::draw(IRenderer& renderer)
	{
		auto vbufferID = renderer.createVertexBuffer(_data.size());
		auto& vbuffer = renderer.getVertexBuffer(vbufferID);
		auto ibufferID = renderer.createIndexBuffer(_data.size()+1);
		auto& ibuffer = renderer.getIndexBuffer(ibufferID);

		std::vector<uint32_t> indices;
		
		if(_isSolid)
		{
			vbuffer.setTopology(Topology::TriangleStrip);
			indices.resize(_data.size());
			for(uint32_t i = 1; i < _data.size(); ++i)
			{
				if(i%2 != 0) //currently, i is odd
				{
					indices[i] = (i+1)/2;
				}
				else //i is even
				{
					indices[i] = _data.size() - i/2;
				}
			}
		}
		else
		{
			vbuffer.setTopology(Topology::LineStrip);
			indices.resize(_data.size()+1);
			//set indices
			for(uint32_t i = 0; i < indices.size()-1; ++i)
			{
				indices[i] = i;
			}
		}

		vbuffer.copyData(_data, _color);
		ibuffer.copyData(indices);

		vbuffer.bind();
		ibuffer.bind();

		renderer.setIdentity();
		renderer.setScaling(Util::toVec3(_scale));
		renderer.applyTransform();

		ibuffer.draw();

		renderer.destroyVertexBuffer(vbufferID);
		renderer.destroyIndexBuffer(ibufferID);
	}

	void Polygon::setSolid(bool isSolid)
	{
		_isSolid = isSolid;
	}

	void Polygon::setEdgeThickness(float thickness)
	{
		_thickness = thickness;
	}

	void Polygon::setColor(const Util::Color& color)
	{
		_color = color;
	}

	void Polygon::setAsLine(const Util::Vec2& p1, const Util::Vec2& p2)
	{
		_data.clear();
		_data.reserve(2);
		_data.push_back(p1);
		_data.push_back(p2);
		_isSolid = false;
	}

	void Polygon::setAsTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3)
	{
		_data.clear();
		_data.reserve(3);
		_data.push_back(p1);
		_data.push_back(p2);
		_data.push_back(p3);
	}

	void Polygon::setAsQuad(float halfWidth, float halfHeight)
	{
		_data.clear();
		_data.reserve(4);
		_data.push_back(Util::Vec2(-halfWidth, -halfHeight));
		_data.push_back(Util::Vec2(-halfWidth, halfHeight));
		_data.push_back(Util::Vec2(halfWidth, halfHeight));
		_data.push_back(Util::Vec2(halfWidth, -halfHeight));
		
	}

	void Polygon::setAsCircle(const Util::Vec2& center, float radius, uint32_t numVertices)
	{
		setAsEllipse(center, radius, radius, numVertices);
	}

	void Polygon::setAsEllipse(const Util::Vec2& center, float xRadius, float yRadius, uint32_t numVertices)
	{
		assert(numVertices >= 4);
		if(numVertices%2 != 0)
		{
			++numVertices;
		}
		
		_data.clear();
		_data.resize(numVertices);
		
		float degreesPerVertex = 360.0f / numVertices;
		_data[0].set(xRadius*Util::Vec2(std::cosf(Deg2Rad(0.0f)), yRadius*std::sinf(Deg2Rad(0.0f))));
		_data[0] += center;
		for(uint32_t i = 1; i <= numVertices/2; ++i)
		{
			float degree = degreesPerVertex*i;
			_data[i].set(xRadius*std::cosf(Deg2Rad(-degree)), yRadius*std::sin(Deg2Rad(-degree)));
			if(i != numVertices/2)
			{
				_data[_data.size()-i].set(center.x+_data[i].x, center.y-_data[i].y);
				_data[_data.size()-i] += center;
			}
			_data[i] += center;
		}
	}

	void Polygon::setManual(const std::vector<Util::Vec2>& data)
	{
		setManual(data.data(), data.size());
	}

	void Polygon::setManual(const Util::Vec2* vertices, uint32_t numVertices)
	{
		_data.clear();
		_data.reserve(numVertices);
		for(uint32_t i=0; i<numVertices; ++i)
		{
			_data.push_back(vertices[i]);
		}
	}

	void Polygon::setScale(float scale)
	{
		_scale.set(scale, scale);
	}
}
