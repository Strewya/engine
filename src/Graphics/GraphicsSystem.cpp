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
	static XMVECTOR convert(const Vec3& v);

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
	Vec2 GraphicsSystem::textSize(uint32_t fontID, const std::string& text) const
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
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		std::vector<Vertex> vertices(2);
		vertices[0].setPosition(p1.x, p1.y, 0);
		vertices[0].setDiffuse(p1Color.r, p1Color.g, p1Color.b, p1Color.a);
		vertices[0].setTextureCoords(-1, -1);
		vertices[1].setPosition(p2.x, p2.y, 0);
		vertices[1].setDiffuse(p2Color.r, p2Color.g, p2Color.b, p2Color.a);
		vertices[1].setTextureCoords(-1, -1);

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 2);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), 2 * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);


		/****** INSTANCE BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = 1;
		dpi.fillColor.y = 1;
		dpi.fillColor.z = 1;
		dpi.fillColor.w = 1;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		
		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawInstanced(2, 1, 0, 0);

		safeRelease(inst);
		safeRelease(vb);
	}

	//*****************************************************************
	//					DRAW MULTILINE
	//*****************************************************************
	void GraphicsSystem::drawMultiline(const Transform& tf, const Vec2* pos, uint32_t count, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		std::vector<Vertex> vertices(count);
		for(uint32_t i = 0; i < count; ++i)
		{
			vertices[i].setPosition(pos[i].x, pos[i].y, 0);
			vertices[i].setDiffuse(1, 1, 1, 1);
			vertices[i].setTextureCoords(-1, -1);
		}

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), count);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), count * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INSTANCE BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawInstanced(count, 1, 0, 0);

		safeRelease(inst);
		safeRelease(vb);
	}

	//*****************************************************************
	//					DRAW POLYGON
	//*****************************************************************
	void GraphicsSystem::drawPolygon(const Transform& tf, const Vec2* pos, uint32_t count, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		std::vector<Vertex> vertices(count);
		for(uint32_t i = 0; i < count; ++i)
		{
			vertices[i].setPosition(pos[i].x, pos[i].y, 0);
			vertices[i].setDiffuse(1, 1, 1, 1);
			vertices[i].setTextureCoords(-1, -1);
		}

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), count);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), count * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INSTANCE BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(tf.rotation);
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;
		
		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawInstanced(count, 1, 0, 0);

		safeRelease(inst);
		safeRelease(vb);
	}

	//*****************************************************************
	//					DRAW QUAD POLYGON
	//*****************************************************************
	void GraphicsSystem::drawQuadPolygon(const Transform& tf, const Rect& rect, const Color& c)
	{
		uint32_t count = 5;
		std::vector<Vec2> vertices(count);
		vertices[0].set(rect.center.x - rect.halfWidth, rect.center.y - rect.halfHeight);
		vertices[1].set(rect.center.x + rect.halfWidth, rect.center.y - rect.halfHeight);
		vertices[2].set(rect.center.x + rect.halfWidth, rect.center.y + rect.halfHeight);
		vertices[3].set(rect.center.x - rect.halfWidth, rect.center.y + rect.halfHeight);
		vertices[4] = vertices[0];

		drawMultiline(tf, vertices.data(), count, c);
	}

	//*****************************************************************
	//					DRAW CIRCLE POLYGON
	//*****************************************************************
	void GraphicsSystem::drawCirclePolygon(const Transform& tf, float radius, uint32_t p, const Color& c)
	{
		if((p & 1) == 1) //if it's odd
		{
			++p; //make it even
		}
		std::vector<Vec2> vertices;
		vertices.reserve(p + 1);
		auto dist = m_circleData.size() / p;
		for(uint32_t i = 0; i < m_circleData.size(); i += dist)
		{
			vertices.emplace_back(m_circleData[i] * radius);
		}
		vertices.emplace_back(vertices.front());
		
		drawMultiline(tf, vertices.data(), vertices.size(), c);
	}
	
	//*****************************************************************
	//					DRAW QUAD
	//*****************************************************************
	void GraphicsSystem::drawQuad(const Transform& tf, const Vec2& hs, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		/*
		0-1
		| |
		2-3
		*/
		Vertex vertices[]
		{
			{-hs.x, hs.y, 0, 1, 1, 1, 1, -1, -1},
			{hs.x, hs.y, 0, 1, 1, 1, 1, -1, -1},
			{-hs.x, -hs.y, 0, 1, 1, 1, 1, -1, -1},
			{hs.x, -hs.y, 0, 1, 1, 1, 1, -1, -1},
		};

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices, 4 * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INDEX BUFER ******/
		uint32_t indices[]
		{
			0, 1, 2,     1, 3, 2,
		};

		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), 6);
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices, 6 * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** INSTANCE BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawIndexedInstanced(6, 1, 0, 0, 0);

		safeRelease(inst);
		safeRelease(ib);
		safeRelease(vb);
	}

	//*****************************************************************
	//					DRAW CIRCLE
	//*****************************************************************
	void GraphicsSystem::drawCircle(const Transform& tf, float r, uint32_t p, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		if((p & 1) == 1) //if it's odd
		{
			++p; //make it even
		}
		/****** VERTEX BUFFER ******/
		uint32_t dist = m_circleData.size() / p;
		std::vector<Vertex> vertices;
		vertices.reserve(p + 1);
		vertices.emplace_back(Vertex{0, 0, 0, 1, 1, 1, 1, -1, -1});
		for(uint32_t i = 0; i < m_circleData.size(); i += dist)
		{
			auto& v = m_circleData[i];
			vertices.emplace_back(Vertex{v.x*r, v.y*r, 0, 1, 1, 1, 1, -1, -1});
		}

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), vertices.size());

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), vertices.size() * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INDEX BUFER ******/
		std::vector<uint32_t> indices;
		indices.reserve(p * 3);
		for(uint32_t i = 1; i <= p; ++i)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back((i % p) + 1);
		}

		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), indices.size());
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices.data(), indices.size() * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** INSTANCE BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);

		safeRelease(inst);
		safeRelease(ib);
		safeRelease(vb);
		m_devcon->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
	}

	//*****************************************************************
	//					DRAW TEXTURED QUAD
	//*****************************************************************
	void GraphicsSystem::drawTexturedQuad(const Transform& tf, const Color& c, const Image& img)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		/*
			0-1
			| |
			2-3
		*/
		Vertex vertices[]
		{
			{-img.m_ratio, 1, 0, 1, 1, 1, 1, img.m_texCoords[0].x, img.m_texCoords[0].y},
			{img.m_ratio, 1, 0, 1, 1, 1, 1, img.m_texCoords[1].x, img.m_texCoords[1].y},
			{-img.m_ratio, -1, 0, 1, 1, 1, 1, img.m_texCoords[2].x, img.m_texCoords[2].y},
			{img.m_ratio, -1, 0, 1, 1, 1, 1, img.m_texCoords[3].x, img.m_texCoords[3].y}
		};

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices, 4 * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INDEX BUFER ******/
		uint32_t indices[]
		{
			0, 1, 2, 1, 3, 2
		};

		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), 6);
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices, 6 * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** CONSTANT BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_devcon->PSSetSamplers(0, 1, &m_samplerState);
		const auto* texture = m_textureCache->getResource(img.m_textureID);
		auto srv = m_textures[texture->m_rawTextureID].get();
		m_devcon->PSSetShaderResources(0, 1, &srv);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawIndexedInstanced(6, 1, 0, 0, 0);

		safeRelease(inst);
		safeRelease(ib);
		safeRelease(vb);
	}

	//*****************************************************************
	//					THE NEW DRAW CALL
	//*****************************************************************
	void GraphicsSystem::theNewDrawCall(const Transform& tf, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const Color& c, uint32_t tId)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), vertices.size());

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), vertices.size() * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		
		/****** PRIMITIVE TOPOLOGY ******/
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		/****** INDEX BUFER ******/
		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), indices.size());
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices.data(), indices.size() * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** CONSTANT BUFFER ******/
		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);
		
		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		
		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);
		if(tId != INVALID_ID)
		{
			m_devcon->PSSetSamplers(0, 1, &m_samplerState);
			const auto* texture = m_textureCache->getResource(tId);
			auto srv = m_textures[texture->m_rawTextureID].get();
			m_devcon->PSSetShaderResources(0, 1, &srv);
		}

		m_devcon->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);

		safeRelease(inst);
		safeRelease(ib);
		safeRelease(vb);
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

	void GraphicsSystem::v3_draw(uint32_t indiceCount, uint32_t instanceCount)
	{
		if(instanceCount >= 1)
		{
			m_devcon->DrawIndexedInstanced(indiceCount, instanceCount, 0, 0, 0);
		}
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

	std::vector<uint32_t> GraphicsSystem::v3_makeSolidCircleIndices(uint32_t p) const
	{
		std::vector<uint32_t> indices;
		indices.reserve(p * 3);
		for(uint32_t i = 1; i <= p; ++i)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back((i % p) + 1);
		}
		return indices;
	}

	std::vector<uint32_t> GraphicsSystem::v3_makeHollowCircleIndices(uint32_t p) const
	{
		std::vector<uint32_t> indices;
		indices.reserve(p * 2);
		for(uint32_t i = 1; i <= p; ++i)
		{
			indices.push_back(i);
			indices.push_back((i % p)+1);
		}
		return indices;
	}

	std::vector<Vertex> GraphicsSystem::v3_makeQuadVertices(const Vec2& pos, const Vec2& hs) const
	{
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

	std::vector<uint32_t> GraphicsSystem::v3_makeHollowQuadVertices() const
	{
		return
		{
			0,1,
			1,2,
			2,3,
			3,0
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
	//					DRAW TEXT
	//*****************************************************************
	void GraphicsSystem::drawText(uint32_t fontID, const std::string& text, const Transform& tf, const Color& c, uint32_t justification, bool isItalic)
	{
		const auto* font = m_fontCache->getResource(fontID);
		if(!font)
		{
			return;
		}
		const auto* texture = m_textureCache->getResource(font->m_textureID);
		if(!texture)
		{
			return;
		}
		float w = texture->m_dimensions.x;
		float h = texture->m_dimensions.y;
		std::vector<Vertex> verts(text.size() * 4);
		std::vector<uint32_t> inds(text.size() * 6);
		uint32_t v = 0;
		uint32_t i = 0;
		float xPos = 0;
		float yPos = font->m_size * 0.5f;

		for(char c : text)
		{
			const Glyph& glyph = font->m_glyphs[c - 32];
			float tv_top = (float)glyph.m_top / h;
			float tv_bot = (float)(glyph.m_top + font->m_size) / h;
			float tu_left = (float)glyph.m_left / w;
			float tu_rght = (float)glyph.m_right / w;

			inds[i++] = v + 0;
			inds[i++] = v + 1;
			inds[i++] = v + 2;
			inds[i++] = v + 2;
			inds[i++] = v + 3;
			inds[i++] = v + 0;

			float italicOffset = isItalic ? 2.5f : 0;

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
		D3D11_MAPPED_SUBRESOURCE ms;

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), verts.size());

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, verts.data(), verts.size() * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		/****** INDEX BUFER ******/
		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), inds.size());
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, inds.data(), inds.size() * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** INSTANCE BUFFER ******/
		float move = 0;
		if(justification == 1)
		{
			move = xPos*0.5f*tf.scale.x;
		}
		else if(justification == 2)
		{
			move = xPos*tf.scale.x;
		}

		auto world = XMMatrixIdentity();
		world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		world *= XMMatrixTranslation(tf.position.x - move, tf.position.y, 0);
		world = XMMatrixTranspose(world*m_camView*m_camProjection);

		dataPerInstance dpi;
		XMStoreFloat4x4(&dpi.world, world);
		dpi.fillColor.x = c.r;
		dpi.fillColor.y = c.g;
		dpi.fillColor.z = c.b;
		dpi.fillColor.w = c.a;

		auto* inst = makeInstanceBuffer(m_dev, sizeof(dataPerInstance), 1);
		hr = m_devcon->Map(inst, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, &dpi, sizeof(dataPerInstance));
		m_devcon->Unmap(inst, 0);

		uint32_t strides[2] = {sizeof(Vertex), sizeof(dataPerInstance)};
		uint32_t offsets[2] = {0, 0};
		ID3D11Buffer* bufferPtrs[2] = {vb, inst};
		m_devcon->IASetVertexBuffers(0, 2, bufferPtrs, strides, offsets);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_devcon->PSSetSamplers(0, 1, &m_samplerState);
		auto srv = m_textures[texture->m_rawTextureID].get();
		m_devcon->PSSetShaderResources(0, 1, &srv);

		m_devcon->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		m_devcon->PSSetConstantBuffers(0, 1, &m_constantBuffer);

		m_devcon->DrawIndexedInstanced(inds.size(), 1, 0, 0, 0);

		safeRelease(inst);
		safeRelease(ib);
		safeRelease(vb);
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

		DEBUG_CODE_START
			if(FAILED(hr))
				DEBUG_INFO("\tinitDevice failed");
		DEBUG_CODE_END

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
		DEBUG_CODE_START
			if(FAILED(hr))
				DEBUG_INFO("\tinitRenderTarget failed");
		DEBUG_CODE_END
			return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT VERTEX SHADER
	//*****************************************************************
	bool GraphicsSystem::initVertexShader(const ResourceFile& shaderFile)
	{
		ID3D10Blob* m_shaderBlob = nullptr;
		HRESULT hr = S_OK;
#ifndef _DEBUG
#include <Graphics/vshader.h>
		const LPVOID shaderBuffer = (LPVOID)g_VShader;
		SIZE_T shaderSize = sizeof(g_VShader);
#else
		hr = D3DX11CompileFromFile(shaderFile.getPath().c_str(), nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &m_shaderBlob, nullptr, nullptr);
		LPVOID shaderBuffer = m_shaderBlob->GetBufferPointer();
		SIZE_T shaderSize = m_shaderBlob->GetBufferSize();

		DEBUG_INFO((int32_t)hr);
		if(SUCCEEDED(hr))
#endif
		{
			safeRelease(m_vertexShader);
			hr = m_dev->CreateVertexShader(shaderBuffer, shaderSize, nullptr, &m_vertexShader);
			DEBUG_INFO((int32_t)hr);
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
			safeRelease(m_shaderBlob);
		}
		DEBUG_CODE_START
			if(FAILED(hr))
			{
				DEBUG_INFO("\tinitVertexShader failed: ", hr);
			}
		DEBUG_CODE_END
			return SUCCEEDED(hr);
	}

	//*****************************************************************
	//					INIT PIXEL SHADER
	//*****************************************************************
	bool GraphicsSystem::initPixelShader(const ResourceFile& shaderFile)
	{
		ID3D10Blob* m_shaderBlob = nullptr;
		HRESULT hr = S_OK;
#ifndef _DEBUG
#include <Graphics/pshader.h>
		const LPVOID shaderBuffer = (LPVOID)g_PShader;
		SIZE_T shaderSize = sizeof(g_PShader);
#else
		hr = D3DX11CompileFromFile(shaderFile.getPath().c_str(), nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &m_shaderBlob, nullptr, nullptr);
		LPVOID shaderBuffer = m_shaderBlob->GetBufferPointer();
		SIZE_T shaderSize = m_shaderBlob->GetBufferSize();
		DEBUG_INFO((int32_t)hr);
		if(SUCCEEDED(hr))
#endif
		{
			safeRelease(m_pixelShader);
			hr = m_dev->CreatePixelShader(shaderBuffer, shaderSize, nullptr, &m_pixelShader);
			DEBUG_INFO((int32_t)hr);
			if(SUCCEEDED(hr))
			{
				m_devcon->PSSetShader(m_pixelShader, nullptr, 0);
			}
			safeRelease(m_shaderBlob);
		}
		DEBUG_CODE_START
			if(FAILED(hr))
			{
				DEBUG_INFO("\tinitPixelShader failed:", hr);
			}
		DEBUG_CODE_END
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

	XMVECTOR convert(const Vec3& v)
	{
		return XMVectorSet(v.x, v.y, v.z, 0);
	}
}