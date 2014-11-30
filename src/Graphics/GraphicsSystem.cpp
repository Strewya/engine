//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/GraphicsSystem.h>
/******* C++ headers *******/
#include <cassert>
#include <iostream>
/******* extra headers *******/
#include <Caches/FontCache.h>
#include <Caches/TextureCache.h>
#include <DataStructs/Font.h>
#include <DataStructs/Image.h>
#include <DataStructs/Texture.h>
#include <Graphics/Camera.h>
#include <Graphics/Vertex.h>
#include <Util/Circle.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Util/Rect.h>
#include <Util/ResourceFile.h>
#include <Util/Vec3.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	struct dataPerScene
	{
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct dataPerInstance
	{
		XMFLOAT4X4 world;
		XMFLOAT4 fillColor;
	};

	static void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd, HWND hwnd, uint32_t width, uint32_t height);
	static ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
	static ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
	static ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, uint32_t unitSize);
	static ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
	static XMVECTOR convert(Vec2 v);
	static XMVECTOR convert(Vec3 v);

	//*****************************************************************
	//					INIT
	//*****************************************************************
	bool GraphicsSystem::init(FontCache& fontCache, TextureCache& textureCache, Window& window)
	{
		DEBUG_STATUS(true);

		declare(&m_dxgiFactory);
		declare(&m_dev);
		declare(&m_devcon);
		declare(&m_swapchain);
		declare(&m_renderTarget);
		declare(&m_vertexShader);
		declare(&m_pixelShader);
		declare(&m_inputLayout);
		declare(&m_samplerState);
		declare(&m_depthStencilView);
		declare(&m_depthStencilBuffer);
		declare(&m_transparency);
		declare(&m_constantBuffer);
		declare(&m_vertexBuffer);

		m_fontCache = &fontCache;
		m_textureCache = &textureCache;
		m_window = &window;
		m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;

		status = initDevice() &&
			initSwapChain() &&
			//initDepthBuffer() &&
			initRenderTarget() &&
			initViewport() &&
			initSamplerState();

		m_camView = XMMatrixIdentity();
		m_constantBuffer = makeConstantBuffer(m_dev, sizeof(dataPerScene));
		status &= (m_constantBuffer != nullptr);

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.AlphaToCoverageEnable = false;

		D3D11_RENDER_TARGET_BLEND_DESC& rtbd = blendDesc.RenderTarget[0];
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = m_dev->CreateBlendState(&blendDesc, &m_transparency);

		setCulling(true);
		setTransparencyMode(false);
		setPerspectiveProjection();

		m_circleData.reserve(360 * 2);
		for(float degree = 0; degree < 360.0f; degree += 0.5f)
		{
			auto rad = XMConvertToRadians(degree);
			m_circleData.emplace_back(std::cosf(rad), std::sinf(rad));
		}

		status &= SUCCEEDED(hr);

		DEBUG_INIT(GraphicsSystem);
	}

	//*****************************************************************
	//					SHUTDOWN
	//*****************************************************************
	bool GraphicsSystem::shutdown()
	{
		DEBUG_STATUS(true);

		if(m_swapchain != nullptr)
		{
			m_swapchain->SetFullscreenState(false, nullptr);
		}

		for(auto** ptr : m_dxInterfaces)
		{
			safeRelease(*ptr);
		}

		DEBUG_SHUTDOWN(GraphicsSystem);
	}

	//*****************************************************************
	//					UPDATE
	//*****************************************************************
	void GraphicsSystem::update()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);
		m_devcon->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		//go through the command list and execute each one
		//each command is a special draw call for whatever needs to be drawn

		m_swapchain->Present(0, 0);
	}

	//*****************************************************************
	//					BEGIN DRAWING
	//*****************************************************************
	void GraphicsSystem::begin()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);
	}

	//*****************************************************************
	//					PRESENT BACKBUFFER
	//*****************************************************************
	void GraphicsSystem::present()
	{
		m_swapchain->Present(0, 0);
	}

	//*****************************************************************
	//					SET BACKGROUND COLOR
	//*****************************************************************
	void GraphicsSystem::setBackgroundColor(float r, float g, float b)
	{
		m_backgroundColor.r = r;
		m_backgroundColor.g = g;
		m_backgroundColor.b = b;
	}

	//*****************************************************************
	//					SET CULLING
	//*****************************************************************
	void GraphicsSystem::setCulling(bool isEnabled)
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));

		if(isEnabled)
		{
			rd.CullMode = D3D11_CULL_BACK;
		}
		else
		{
			rd.CullMode = D3D11_CULL_NONE;
		}
		rd.FillMode = D3D11_FILL_SOLID;


		ID3D11RasterizerState* state = nullptr;

		HRESULT hr = m_dev->CreateRasterizerState(&rd, &state);
		if(SUCCEEDED(hr))
		{
			m_devcon->RSSetState(state);
			safeRelease(state);
		}
	}

	//*****************************************************************
	//					SET TRANSPARENCY MODE
	//*****************************************************************
	void GraphicsSystem::setTransparencyMode(bool isEnabled)
	{
		auto transpEnabled = m_transparency;
		if(!isEnabled)
		{
			transpEnabled = nullptr;
		}
		m_devcon->OMSetBlendState(transpEnabled, nullptr, 0xffffffff);
	}

	//*****************************************************************
	//					SET ORTHOGRAPHIC PROJECTION
	//*****************************************************************
	void GraphicsSystem::setOrthographicProjection()
	{
		m_camProjection = XMMatrixOrthographicLH((float)m_window->getSizeX(), (float)m_window->getSizeY(), 1.0f, 100.0f);
		dataPerScene dps;
		dps.projection = XMMatrixTranspose(m_camProjection);
		m_devcon->UpdateSubresource(m_constantBuffer, 0, nullptr, &dps, 0, 0);
	}

	//*****************************************************************
	//					SET PERSPECTIVE PROJECTION
	//*****************************************************************
	void GraphicsSystem::setPerspectiveProjection()
	{
		m_camProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)m_window->getSizeX() / m_window->getSizeY(), 1.0f, 100.0f);
		dataPerScene dps;
		dps.projection = XMMatrixTranspose(m_camProjection);
		m_devcon->UpdateSubresource(m_constantBuffer, 0, nullptr, &dps, 0, 0);
	}

	//*****************************************************************
	//					CALCULATE CAM VIEW
	//*****************************************************************
	XMMATRIX GraphicsSystem::calculateCamView(const Camera& cam) const
	{
		auto pos = convert(cam.getPosition());
		auto lookAt = convert(cam.getLookAtAxis());
		auto up = convert(cam.getUpAxis());
		auto rot = convert(cam.getRotation()*cam.getSpeed());

		auto rotMat = XMMatrixRotationRollPitchYawFromVector(rot);
		rot = XMVector3Transform(lookAt, rotMat);
		lookAt = rot + pos;

		return XMMatrixLookAtLH(pos, lookAt, up);
	}

	//*****************************************************************
	//					APPLY CAMERA
	//*****************************************************************
	void GraphicsSystem::applyCamera(const Camera& cam)
	{
		m_camView = calculateCamView(cam);
		dataPerScene dps;
		dps.view = XMMatrixTranspose(m_camView);
		m_devcon->UpdateSubresource(m_constantBuffer, 0, nullptr, &dps, 0, 0);
	}

	//*****************************************************************
	//					CLEAR CAMERA
	//*****************************************************************
	void GraphicsSystem::clearCamera()
	{
		m_camView = XMMatrixIdentity();
		dataPerScene dps;
		dps.view = XMMatrixTranspose(m_camView);
		m_devcon->UpdateSubresource(m_constantBuffer, 0, nullptr, &dps, 0, 0);
	}

	//*****************************************************************
	//					SCREEN TO WORLD COORDS
	//*****************************************************************
	Vec2 GraphicsSystem::screenToWorld(const Vec2& screen, const Camera& cam) const
	{
		auto camView = calculateCamView(cam);
		
		auto objectSpace = XMVector3Unproject(convert(Vec3{screen.x, screen.y, 0.0f}), 0, 0, (float)m_window->getSizeX(), (float)m_window->getSizeY(), 0.0f, 1.0f, m_camProjection, camView, XMMatrixIdentity());
		auto camPos = convert(cam.getPosition());
		auto plane = XMPlaneFromPoints(convert({0, 0, 0}), convert({1, 0, 0}), convert({0, 1, 0}));
		auto loc = XMPlaneIntersectLine(plane, objectSpace, camPos);
		return Vec2{loc.m128_f32[0], loc.m128_f32[1]};
	}

	//*****************************************************************
	//					TEXT SIZE
	//*****************************************************************
	Vec2 GraphicsSystem::textHalfSize(uint32_t fontID, const std::string& text) const
	{
		const auto* font = m_fontCache->getResource(fontID);
		if(!font)
		{
			return{};
		}
		float width = 0;
		auto height = (float)font->m_size;

		for(char c : text)
		{
			const Glyph& glyph = font->m_glyphs[c - 32];
			
			width += (float)(glyph.m_right - glyph.m_left);
		}
		return{width*0.5f, height*0.5f};
	}

	//*****************************************************************
	//					JUSTIFY TEXT
	//*****************************************************************
	Transform GraphicsSystem::justifyText(Transform tf, float textSizeX, TextJustification justification)
	{
		float move = 0;
		switch( justification )
		{
			case TextJustification::TJ_Center:
			{
				move = textSizeX*0.5f*tf.scale.x;
			} break;

			case TextJustification::TJ_Right:
			{
				move = textSizeX*tf.scale.x;
			} break;
		}
		tf.position.x -= move;
		return tf;
	}

	//*****************************************************************
	//					GET TEXTURE DIMENSIONS
	//*****************************************************************
	Vec2 GraphicsSystem::getTextureDimensions(const Texture& texture) const
	{
		ID3D11Resource* res = nullptr;
		m_textures[texture.m_rawTextureID]->GetResource(&res);

		ID3D11Texture2D* texture2d = nullptr;
		HRESULT hr = res->QueryInterface(&texture2d);

		Vec2 dim(0, 0);
		if(SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			texture2d->GetDesc(&desc);
			dim.x = static_cast<float>(desc.Width);
			dim.y = static_cast<float>(desc.Height);
		}
		safeRelease(texture2d);
		safeRelease(res);
		return dim;
	}

	//*****************************************************************
	//					DRAW LINE
	//*****************************************************************
	void GraphicsSystem::drawLine(const Transform& tf, const Vec2& p1, const Vec2& p2, const Color& c)
	{
		drawLine(tf, p1, c, p2, c);
	}

	//*****************************************************************
	//					DRAW LINE
	//*****************************************************************
	void GraphicsSystem::drawLine(const Transform& tf, const Vec2& p1, const Color& p1Color, const Vec2& p2, const Color& p2Color)
	{
		std::vector<Vertex> vertices(2);
		vertices[0].setPosition(p1.x, p1.y, 0);
		vertices[0].setDiffuse(p1Color.r, p1Color.g, p1Color.b, p1Color.a);
		vertices[0].setTextureCoords(-1, -1);
		vertices[1].setPosition(p2.x, p2.y, 0);
		vertices[1].setDiffuse(p2Color.r, p2Color.g, p2Color.b, p2Color.a);
		vertices[1].setTextureCoords(-1, -1);

		v3_setVertices(vertices);

		v3_setInstanceData({tf}, {{}}, 0, 1);

		v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		
		m_devcon->DrawInstanced(2, 1, 0, 0);
	}

	//*****************************************************************
	//					DRAW MULTILINE
	//*****************************************************************
	void GraphicsSystem::drawMultiline(const Transform& tf, const Vec2* pos, uint32_t count, const Color& c)
	{
		std::vector<Vertex> vertices(count);
		for(uint32_t i = 0; i < count; ++i)
		{
			vertices[i].setPosition(pos[i].x, pos[i].y, 0);
			vertices[i].setDiffuse(1, 1, 1, 1);
			vertices[i].setTextureCoords(-1, -1);
		}

		v3_setVertices(vertices);

		v3_setInstanceData({tf}, {c}, 0, 1);
		v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		m_devcon->DrawInstanced(count, 1, 0, 0);
	}

	

	void GraphicsSystem::v3_setVertices(const std::vector<Vertex>& vertices)
	{
		if(vertices.size() == 0)
			return;

		D3D11_MAPPED_SUBRESOURCE ms;
		safeRelease(m_vertexBuffer);
		m_vertexBuffer = makeVertexBuffer(m_dev, sizeof(Vertex), vertices.size());

		HRESULT hr = m_devcon->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), vertices.size() * sizeof(Vertex));
		m_devcon->Unmap(m_vertexBuffer, 0);
	}

	void GraphicsSystem::v3_setIndices(const std::vector<uint32_t>& indices)
	{
		if(indices.size() == 0)
			return;

		D3D11_MAPPED_SUBRESOURCE ms;
		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), indices.size());
		HRESULT hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices.data(), indices.size() * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		safeRelease(ib);
	}

	void GraphicsSystem::v3_setTopology(D3D_PRIMITIVE_TOPOLOGY topology)
	{
		m_devcon->IASetPrimitiveTopology(topology);
	}

	void GraphicsSystem::v3_setInstanceData(const std::vector<Transform>& tfs, const std::vector<Color>& fills, uint32_t startOffset, uint32_t count)
	{
		if(count == 0)
			return;
		
		assert(startOffset < tfs.size());
		assert(startOffset + count <= tfs.size());
		assert(tfs.size() == fills.size());
		
		std::vector<dataPerInstance> data;
		data.reserve(count);
		for(uint32_t i = startOffset; i < startOffset+count; ++i)
		{
			data.emplace_back();
			auto& dpi = data.back();
			
			dpi.fillColor.x = fills[i].r;
			dpi.fillColor.y = fills[i].g;
			dpi.fillColor.z = fills[i].b;
			dpi.fillColor.w = fills[i].a;

			auto world = XMMatrixIdentity();
			world *= XMMatrixScaling(tfs[i].scale.x, tfs[i].scale.y, 1.0f);
			//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
			//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
			world *= XMMatrixRotationZ(tfs[i].rotation);
			world *= XMMatrixTranslation(tfs[i].position.x, tfs[i].position.y, 0);
			world = XMMatrixTranspose(world*m_camView*m_camProjection);

			XMStoreFloat4x4(&dpi.world, world);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		auto* ib = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), count);
		HRESULT hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, data.data(), data.size() * sizeof(dataPerInstance));
		m_devcon->Unmap(ib, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {m_vertexBuffer, ib};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);

		safeRelease(ib);
		safeRelease(m_vertexBuffer);
	}

	void GraphicsSystem::v3_setTexture(uint32_t textureId)
	{
		m_devcon->PSSetSamplers(0, 1, &m_samplerState);
		const auto* texture = m_textureCache->getResource(textureId);
		auto srv = m_textures[texture->m_rawTextureID].get();
		m_devcon->PSSetShaderResources(0, 1, &srv);
	}

	void GraphicsSystem::v3_setFontTexture(uint32_t fontId)
	{
		const auto* font = m_fontCache->getResource(fontId);
		if( font )
		{
			v3_setTexture(font->m_textureID);
		}
	}

	void GraphicsSystem::v3_draw(uint32_t indiceCount, uint32_t instanceCount)
	{
		if(instanceCount >= 1)
		{
			m_devcon->DrawIndexedInstanced(indiceCount, instanceCount, 0, 0, 0);
		}
	}

	std::vector<Vertex> GraphicsSystem::v3_makeCustomVertices(const Vec2* pos, uint32_t pts) const
	{
		std::vector<Vertex> vertices;
		vertices.reserve(pts + 1);
		vertices.emplace_back(Vertex{0, 0, 0, 1, 1, 1, 1, -1, -1});
		Vec2 center;
		for( uint32_t i = 0; i < pts; ++i )
		{
			auto& v = pos[i];
			center += v;
			vertices.emplace_back(Vertex{v.x, v.y, 0, 1, 1, 1, 1, -1, -1});
		}
		center /= (float)pts;
		vertices[0].setPosition(center.x, center.y, 0);
		return vertices;
	}

	std::vector<Vertex> GraphicsSystem::v3_makeCircleVertices(const Vec2& pos, float r, uint32_t pts) const
	{
		if((pts & 1) == 1) //if it's odd
		{
			++pts; //make it even
		}

		uint32_t dist = m_circleData.size() / pts;
		std::vector<Vertex> vertices;
		vertices.reserve(pts + 1);
		vertices.emplace_back(Vertex{pos.x, pos.y, 0, 1, 1, 1, 1, -1, -1});
		for(uint32_t i = 0; i < m_circleData.size(); i += dist)
		{
			auto& v = m_circleData[i];
			vertices.emplace_back(Vertex{pos.x+v.x*r, pos.y+v.y*r, 0, 1, 1, 1, 1, -1, -1});
		}
		return vertices;
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeSolidPolygonIndices(uint32_t p) const
	{
		std::vector<uint32_t> indices;
		indices.reserve(p * 3);
		for( uint32_t i = 1; i <= p; ++i )
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back((i % p) + 1);
		}
		return indices;
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeHollowPolygonIndices(uint32_t p) const
	{
		std::vector<uint32_t> indices;
		indices.reserve(p * 2);
		for( uint32_t i = 1; i <= p; ++i )
		{
			indices.push_back(i);
			indices.push_back((i % p) + 1);
		}
		return indices;
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeSolidCircleIndices(uint32_t p) const
	{
		if( (p & 1) == 1 ) //if it's odd
		{
			++p; //make it even
		}

		return v3_makeSolidPolygonIndices(p);
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeHollowCircleIndices(uint32_t p) const
	{
		if( (p & 1) == 1 ) //if it's odd
		{
			++p; //make it even
		}
		return v3_makeHollowPolygonIndices(p);
	}

	std::vector<Vertex> GraphicsSystem::v3_makeQuadVertices(const Vec2& pos, const Vec2& hs) const
	{
		/*
			0-1
			| |
			2-3
		*/
		return
		{
			{pos.x-hs.x, pos.y+hs.y, 0, 1, 1, 1, 1, -1, -1},
			{pos.x+hs.x, pos.y+hs.y, 0, 1, 1, 1, 1, -1, -1},
			{pos.x-hs.x, pos.y-hs.y, 0, 1, 1, 1, 1, -1, -1},
			{pos.x+hs.x, pos.y-hs.y, 0, 1, 1, 1, 1, -1, -1},
		};
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeSolidQuadIndices() const
	{
		return
		{
			0, 1, 3,
			0, 3, 2
		};
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeHollowQuadIndices() const
	{
		return
		{
			0,1,
			1,3,
			3,2,
			2,0
		};
	}

	std::vector<Vertex> GraphicsSystem::v3_makeTextVertices(uint32_t fontID, const std::string& text, bool italic)
	{
		const auto* font = m_fontCache->getResource(fontID);
		if(!font)
		{
			return{};
		}
		const auto* texture = m_textureCache->getResource(font->m_textureID);
		if(!texture)
		{
			return{};
		}
		float w = texture->m_dimensions.x;
		float h = texture->m_dimensions.y;
		std::vector<Vertex> verts(text.size() * 4);
		uint32_t v = 0;
		float xPos = 0;
		float yPos = font->m_size * 0.5f;

		for(char c : text)
		{
			const Glyph& glyph = font->m_glyphs[c - 32];
			float tv_top = (float)glyph.m_top / h;
			float tv_bot = (float)(glyph.m_top + font->m_size) / h;
			float tu_left = (float)glyph.m_left / w;
			float tu_rght = (float)glyph.m_right / w;

			float italicOffset = italic ? 2.5f : 0;

			verts[v].setPosition(xPos - italicOffset, -yPos, 0);
			verts[v].setDiffuse(1, 1, 1, 1);
			verts[v].setTextureCoords(tu_left, tv_bot);
			++v;
			verts[v].setPosition(xPos + italicOffset, yPos, 0);
			verts[v].setDiffuse(1, 1, 1, 1);
			verts[v].setTextureCoords(tu_left, tv_top);
			++v;
			xPos += (glyph.m_right - glyph.m_left);
			verts[v].setPosition(xPos + italicOffset, yPos, 0);
			verts[v].setDiffuse(1, 1, 1, 1);
			verts[v].setTextureCoords(tu_rght, tv_top);
			++v;
			verts[v].setPosition(xPos - italicOffset, -yPos, 0);
			verts[v].setDiffuse(1, 1, 1, 1);
			verts[v].setTextureCoords(tu_rght, tv_bot);
			++v;
		}
		return verts;
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeTextIndices(uint32_t letters) const
	{
		std::vector<uint32_t> inds;
		inds.reserve(letters * 6);
		for(uint32_t i = 0; i < letters; ++i)
		{
			auto x = i * 4;
			inds.emplace_back(x + 0);
			inds.emplace_back(x + 1);
			inds.emplace_back(x + 2);
			inds.emplace_back(x + 2);
			inds.emplace_back(x + 3);
			inds.emplace_back(x + 0);
		}
		return inds;
	}

	//*****************************************************************
	//					INIT DEVICE
	//*****************************************************************
	bool GraphicsSystem::initDevice()
	{
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

		if(SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = m_dxgiFactory->EnumAdapters(0, &adapter);
			if(SUCCEEDED(hr))
			{
				auto driverType = D3D_DRIVER_TYPE_UNKNOWN;
				uint32_t flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef _DEBUG
					flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

					hr = D3D11CreateDevice(adapter, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_dev, nullptr, &m_devcon);

				adapter->Release();
			}
		}

		if(FAILED(hr))
		{
			DEBUG_INFO("\tinitDevice failed");
		}
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT SWAP CHAIN
	//*****************************************************************
	bool GraphicsSystem::initSwapChain()
	{
		fillSwapChainDesc(m_swapChainDesc, m_window->getWindowHandle(), m_window->getSizeX(), m_window->getSizeY());

		HRESULT hr = m_dxgiFactory->CreateSwapChain(m_dev, &m_swapChainDesc, &m_swapchain);
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT RENDER TARGET
	//*****************************************************************
	bool GraphicsSystem::initRenderTarget()
	{
		ID3D11Texture2D* bbTexture = nullptr;
		HRESULT hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bbTexture);
		if(SUCCEEDED(hr))
		{
			hr = m_dev->CreateRenderTargetView(bbTexture, nullptr, &m_renderTarget);
			if(SUCCEEDED(hr))
			{
				m_devcon->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);
			}
			bbTexture->Release();
		}
		
		if(FAILED(hr))
		{
			DEBUG_INFO("\tinitRenderTarget failed");
		}
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT VERTEX SHADER
	//*****************************************************************
	bool GraphicsSystem::initVertexShader(const ResourceFile& shaderFile)
	{
		LPVOID shaderBuffer = nullptr;
		SIZE_T shaderSize = 0;
		{
			#include <Graphics/vshader.h>
			shaderBuffer = (LPVOID)g_VShader;
			shaderSize = sizeof(g_VShader);
		}
		
		safeRelease(m_vertexShader);
		HRESULT hr = m_dev->CreateVertexShader(shaderBuffer, shaderSize, nullptr, &m_vertexShader);
		if(SUCCEEDED(hr))
		{
			auto& ied = Vertex::getDescription();
			safeRelease(m_inputLayout);
			hr = m_dev->CreateInputLayout(ied.data(), ied.size(), shaderBuffer, shaderSize, &m_inputLayout);
			DEBUG_INFO((int32_t)hr);
			if(SUCCEEDED(hr))
			{
				m_devcon->VSSetShader(m_vertexShader, nullptr, 0);
				m_devcon->IASetInputLayout(m_inputLayout);
			}
		}
		
		if(FAILED(hr))
		{
			DEBUG_INFO("\tinitVertexShader failed: ", hr);
		}
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT PIXEL SHADER
	//*****************************************************************
	bool GraphicsSystem::initPixelShader(const ResourceFile& shaderFile)
	{
		LPVOID shaderBuffer = nullptr;
		SIZE_T shaderSize = 0;
		{
			#include <Graphics/pshader.h>
			shaderBuffer = (LPVOID)g_PShader;
			shaderSize = sizeof(g_PShader);
		}

		safeRelease(m_pixelShader);
		HRESULT hr = m_dev->CreatePixelShader(shaderBuffer, shaderSize, nullptr, &m_pixelShader);
		if(SUCCEEDED(hr))
		{
			m_devcon->PSSetShader(m_pixelShader, nullptr, 0);
		}
		
		if(FAILED(hr))
		{
			DEBUG_INFO("\tinitPixelShader failed:", hr);
		}
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT VIEWPORT
	//*****************************************************************
	bool GraphicsSystem::initViewport()
	{
		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = (float)m_window->getSizeX();
		vp.Height = (float)m_window->getSizeY();
		vp.MinDepth = 0;
		vp.MaxDepth = 1;

		m_devcon->RSSetViewports(1, &vp);

		return true;
	}

	//*****************************************************************
	//					INIT SAMPLER STATE
	//*****************************************************************
	bool GraphicsSystem::initSamplerState()
	{
		D3D11_SAMPLER_DESC sampd;
		ZeroMemory(&sampd, sizeof(D3D11_SAMPLER_DESC));
		sampd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampd.MinLOD = 0;
		sampd.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = m_dev->CreateSamplerState(&sampd, &m_samplerState);

		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT DEPTH BUFFER
	//*****************************************************************
	bool GraphicsSystem::initDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC dsd;
		ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));

		dsd.Width = m_window->getSizeX();
		dsd.Height = m_window->getSizeY();
		dsd.MipLevels = 1;
		dsd.ArraySize = 1;
		dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.Usage = D3D11_USAGE_DEFAULT;
		dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags = 0;
		dsd.MiscFlags = 0;

		DXGI_SAMPLE_DESC& sd = dsd.SampleDesc;
		sd.Count = 1;
		sd.Quality = 0;

		auto hr = m_dev->CreateTexture2D(&dsd, nullptr, &m_depthStencilBuffer);
		if(SUCCEEDED(hr))
		{
			hr = m_dev->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);
		}
		return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					LOAD TEXTURE
	//*****************************************************************
	bool GraphicsSystem::loadTexture(const ResourceFile& file, Texture& outTexture)
	{
		ID3D11ShaderResourceView* texturePtr = nullptr;
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, file.getPath().c_str(), nullptr, nullptr, &texturePtr, nullptr);
		bool loaded = false;
		if(SUCCEEDED(hr))
		{
			DxTexturePtr loadedTexturePtr(texturePtr, releasePtr<ID3D11ShaderResourceView>);
			auto it = std::find_if(m_textures.begin(), m_textures.end(), [](const DxTexturePtr& ptr) { return ptr == nullptr; });
			if(it == m_textures.end())
			{
				outTexture.m_rawTextureID = m_textures.size();
				m_textures.emplace_back(std::move(loadedTexturePtr));
			}
			else
			{
				outTexture.m_rawTextureID = std::distance(m_textures.begin(), it);
				it->swap(loadedTexturePtr);
			}
			outTexture.m_name = file.getName();
			outTexture.m_fileHash = file.getHash();
			outTexture.m_dimensions = getTextureDimensions(outTexture);
			loaded = true;
		}
		return loaded;
	}

	//*****************************************************************
	//					RELOAD TEXTURE
	//*****************************************************************
	bool GraphicsSystem::reloadTexture(const ResourceFile& file, Texture& outTexture)
	{
		unloadTexture(outTexture);
		return loadTexture(file, outTexture);
	}

	//*****************************************************************
	//					UNLOAD TEXTURE
	//*****************************************************************
	void GraphicsSystem::unloadTexture(Texture& texture)
	{
		assert(texture.m_rawTextureID < m_textures.size());
		m_textures[texture.m_rawTextureID].reset(nullptr);
		texture.m_rawTextureID = 0;
		texture.m_name.clear();
	}

	//*****************************************************************
	//					MAKE INDEX BUFFER
	//*****************************************************************
	ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
	{
		assert(unitSize > 0 && unitCount > 0);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = unitSize*unitCount;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}


	//*****************************************************************
	//					MAKE VERTEX BUFFER
	//*****************************************************************
	ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
	{
		assert(unitSize > 0 && unitCount > 0);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = unitSize*unitCount;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}

	//*****************************************************************
	//					MAKE CONSTANT BUFFER
	//*****************************************************************
	ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, uint32_t unitSize)
	{
		assert(unitSize > 0);

		D3D11_BUFFER_DESC desc;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = unitSize;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}

	//*****************************************************************
	//					MAKE INSTANCE BUFFER
	//*****************************************************************
	ID3D11Buffer* makeInstanceBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount)
	{
		assert(unitSize > 0);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = unitSize*unitCount;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		ID3D11Buffer* buffer = nullptr;
		HRESULT hr = dev->CreateBuffer(&desc, nullptr, &buffer);
		assert(SUCCEEDED(hr));
		return buffer;
	}

	//*****************************************************************
	//					FILL SWAP CHAIN
	//*****************************************************************
	void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd, HWND hwnd, uint32_t width, uint32_t height)
	{
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferCount = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.OutputWindow = hwnd;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed = true;

		DXGI_MODE_DESC& bd = scd.BufferDesc;
		bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bd.Width = width;
		bd.Height = height;
		bd.RefreshRate.Denominator = 1;
		bd.RefreshRate.Numerator = 60;
		bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC& sd = scd.SampleDesc;
		sd.Count = 1;
		sd.Quality = 0;
	}

	XMVECTOR convert(Vec3 v)
	{
		return XMVectorSet(v.x, v.y, v.z, 0);
	}

	XMVECTOR convert(Vec2 v)
	{
		return XMVectorSet(v.x, v.y, 0, 0);
	}

	void drawHollowPolygon(GraphicsSystem& gfx, Transform transform, const Vec2* positions, uint32_t count, Color color)
	{
		auto vertices = gfx.v3_makeCustomVertices(positions, count);
		auto indices = gfx.v3_makeHollowPolygonIndices(count);

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		gfx.v3_draw(indices.size(), 1);
	}
	
	void drawSolidPolygon(GraphicsSystem& gfx, Transform transform, const Vec2* positions, uint32_t count, Color color)
	{
		auto vertices = gfx.v3_makeCustomVertices(positions, count);
		auto indices = gfx.v3_makeSolidPolygonIndices(count);

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.v3_draw(indices.size(), 1);
	}

	void drawHollowQuad(GraphicsSystem& gfx, Transform transform, Vec2 halfSize, Color color)
	{
		auto vertices = gfx.v3_makeQuadVertices({}, halfSize);
		auto indices = gfx.v3_makeHollowQuadIndices();

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		gfx.v3_draw(indices.size(), 1);
	}

	void drawSolidQuad(GraphicsSystem& gfx, Transform transform, Vec2 halfSize, Color color)
	{
		auto vertices = gfx.v3_makeQuadVertices({}, halfSize);
		auto indices = gfx.v3_makeSolidQuadIndices();

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.v3_draw(indices.size(), 1);
	}

	void drawHollowCircle(GraphicsSystem& gfx, Transform transform, float radius, uint32_t points, Color color)
	{
		auto vertices = gfx.v3_makeCircleVertices({}, radius, points);
		auto indices = gfx.v3_makeHollowCircleIndices(points);

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		gfx.v3_draw(indices.size(), 1);
	}
	
	void drawSolidCircle(GraphicsSystem& gfx, Transform transform, float radius, uint32_t points, Color color)
	{
		auto vertices = gfx.v3_makeCircleVertices({}, radius, points);
		auto indices = gfx.v3_makeSolidCircleIndices(points);

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({transform}, {color}, 0, 1);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.v3_draw(indices.size(), 1);
	}
	
	void drawText(GraphicsSystem& gfx, uint32_t fontID, const std::string& text, Transform transform, Color color, TextJustification justification, bool isItalic)
	{
		auto vertices = gfx.v3_makeTextVertices(fontID, text, isItalic);
		auto indices = gfx.v3_makeTextIndices(text.size());

		gfx.v3_setVertices(vertices);
		gfx.v3_setIndices(indices);
		gfx.v3_setInstanceData({gfx.justifyText(transform, gfx.textHalfSize(fontID, text).x*2, justification)}, {color}, 0, 1);
		gfx.v3_setFontTexture(fontID);
		gfx.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx.v3_draw(indices.size(), 1);
	}
}