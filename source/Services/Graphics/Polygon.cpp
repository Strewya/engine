//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** precompiled header ***/
#include <stdafx.h>
	/*** personal header ***/
#include <Services/Graphics/Polygon.h>
	/*** C++ headers ***/
	/*** extra headers ***/
#include <Services/Graphics/Graphics.h>
#include <Services/Graphics/IRenderer.h>
#include <Services/Graphics/IIndexBuffer.h>
#include <Services/Graphics/IVertexBuffer.h>
#include <Util/Color.h>
	/*** end headers ***/
	
namespace Graphics
{
	Polygon::Polygon()
		: m_color(1,1,1,1), m_data(), m_isSolid(false), m_scale(1,1), m_thickness(1)
	{
	}

	void Polygon::draw(IRenderer& renderer)
	{
		auto vbufferID = renderer.createVertexBuffer(m_data.size());
		auto& vbuffer = renderer.getVertexBuffer(vbufferID);
		auto ibufferID = renderer.createIndexBuffer(m_data.size()+1);
		auto& ibuffer = renderer.getIndexBuffer(ibufferID);

		std::vector<uint32_t> indices;
		
		if(m_isSolid)
		{
			vbuffer.setTopology(Topology::TriangleStrip);
			indices.resize(m_data.size());
			for(uint32_t i = 1; i < m_data.size(); ++i)
			{
				if(i%2 != 0) //currently, i is odd
				{
					indices[i] = (i+1)/2;
				}
				else //i is even
				{
					indices[i] = m_data.size() - i/2;
				}
			}
		}
		else
		{
			vbuffer.setTopology(Topology::LineStrip);
			indices.resize(m_data.size()+1);
			//set indices
			for(uint32_t i = 0; i < indices.size()-1; ++i)
			{
				indices[i] = i;
			}
		}

		vbuffer.copyData(m_data, m_color);
		ibuffer.copyData(indices);

		vbuffer.bind();
		ibuffer.bind();

		renderer.setIdentity();
		renderer.setScaling(Util::toVec3(m_scale));
		renderer.applyTransform();

		ibuffer.draw();

		renderer.destroyVertexBuffer(vbufferID);
		renderer.destroyIndexBuffer(ibufferID);
	}

	void Polygon::setSolid(bool isSolid)
	{
		m_isSolid = isSolid;
	}

	void Polygon::setEdgeThickness(float thickness)
	{
		m_thickness = thickness;
	}

	void Polygon::setColor(const Util::Color& color)
	{
		m_color = color;
	}

	void Polygon::setAsLine(const Util::Vec2& p1, const Util::Vec2& p2)
	{
		m_data.clear();
		m_data.reserve(2);
		m_data.push_back(p1);
		m_data.push_back(p2);
		m_isSolid = false;
	}

	void Polygon::setAsTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3)
	{
		m_data.clear();
		m_data.reserve(3);
		m_data.push_back(p1);
		m_data.push_back(p2);
		m_data.push_back(p3);
	}

	void Polygon::setAsQuad(float halfWidth, float halfHeight)
	{
		m_data.clear();
		m_data.reserve(4);
		m_data.push_back(Util::Vec2(-halfWidth, -halfHeight));
		m_data.push_back(Util::Vec2(halfWidth, -halfHeight));
		m_data.push_back(Util::Vec2(halfWidth, halfHeight));
		m_data.push_back(Util::Vec2(-halfWidth, halfHeight));
		
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
		
		m_data.clear();
		m_data.resize(numVertices);
		
		float degreesPerVertex = 360.0f / numVertices;
		m_data[0].set(xRadius*Util::Vec2(std::cosf(Deg2Rad(0.0f)), yRadius*std::sinf(Deg2Rad(0.0f))));
		m_data[0] += center;
		for(uint32_t i = 1; i <= numVertices/2; ++i)
		{
			float degree = degreesPerVertex*i;
			m_data[i].set(xRadius*std::cosf(Deg2Rad(degree)), yRadius*std::sin(Deg2Rad(degree)));
			if(i != numVertices/2)
			{
				m_data[m_data.size()-i].set(m_data[i].x, -m_data[i].y);
				m_data[m_data.size()-i] += center;
			}
			m_data[i] += center;
		}
	}

	void Polygon::setManual(const std::vector<Util::Vec2>& data)
	{
		setManual(data.data(), data.size());
	}

	void Polygon::setManual(const Util::Vec2* vertices, uint32_t numVertices)
	{
		m_data.clear();
		m_data.reserve(numVertices);
		for(uint32_t i=0; i<numVertices; ++i)
		{
			m_data.push_back(vertices[i]);
		}
	}

	void Polygon::setScale(float scale)
	{
		m_scale.set(scale, scale);
	}
}
