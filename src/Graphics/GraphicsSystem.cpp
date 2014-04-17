//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/GraphicsSystem.h>
/******* C++ headers *******/
#include <cassert>
#include <iostream>
/******* extra headers *******/
#include <DataStructs/Image.h>
#include <Graphics/Vertex.h>
#include <Util/Color.h>
#include <Util/DataFile.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
	struct cbPerObject
	{
		XMMATRIX WVP;
		XMFLOAT4 FillColor;
		XMFLOAT4 isTexture;
	};

	static void fillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& scd, HWND hwnd, uint32_t width, uint32_t height);
	static ID3D11Buffer* makeVertexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
	static ID3D11Buffer* makeIndexBuffer(ID3D11Device* dev, uint32_t unitSize, uint32_t unitCount);
	static ID3D11Buffer* makeConstantBuffer(ID3D11Device* dev, uint32_t unitSize);

	//*****************************************************************
	//					INIT
	//*****************************************************************
	bool GraphicsSystem::init(Window& window)
	{
		bool status = true;

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

		m_window = &window;
		m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;
		
		status = initDevice() &&
			initSwapChain() &&
			//initDepthBuffer() &&
			initRenderTarget() &&
			initViewport() &&
			initSamplerState();

		m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_camLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_camPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		
		m_camView = XMMatrixLookAtLH(m_camPosition, m_camLookAt, m_camUp);
		
		setPerspectiveProjection();

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

		status &= SUCCEEDED(hr);

		DEBUG_INIT(GraphicsSystem);
		return status;
	}

	//*****************************************************************
	//					SHUTDOWN
	//*****************************************************************
	bool GraphicsSystem::shutdown()
	{
		bool status = true;

		if(m_swapchain != nullptr)
		{
			m_swapchain->SetFullscreenState(false, nullptr);
		}

		for(auto** ptr : m_dxInterfaces)
		{
			safeRelease(*ptr);
		}

		DEBUG_SHUTDOWN(GraphicsSystem);
		return status;
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
			DEBUG_INFO("Culling is now ", isEnabled ? "on" : "off");
			safeRelease(state);
		}
		else
		{
			DEBUG_INFO("Failed to create rasterizer state! Code: ", hr);
		}
	}

	//*****************************************************************
	//					SET TRANSPAERNCY MODE
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
	}

	//*****************************************************************
	//					SET PERSPECTIVE PROJECTION
	//*****************************************************************
	void GraphicsSystem::setPerspectiveProjection()
	{
		m_camProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)m_window->getSizeX() / m_window->getSizeY(), 1.0f, 100.0f);
	}

	//*****************************************************************
	//					MOVE CAMERA
	//*****************************************************************
	void GraphicsSystem::moveCamera(const Vec2& translation, bool isAbsolute)
	{
		if(isAbsolute)
		{
			m_camPosition = XMVectorSet(translation.x, translation.y, -1.0f, 0.0f);
			m_camLookAt = XMVectorSet(translation.x, translation.y, 0.0f, 0.0f);
		}
		else
		{
			m_camPosition += XMVectorSet(translation.x, translation.y, 0.0f, 0.0f);
			m_camLookAt += XMVectorSet(translation.x, translation.y, 0.0f, 0.0f);
		}
		m_camView = XMMatrixLookAtLH(m_camPosition, m_camLookAt, m_camUp);
	}

	//*****************************************************************
	//					GET TEXTURE DIMENSIONS
	//*****************************************************************
	Vec2 GraphicsSystem::getTextureDimensions(uint32_t texID) const
	{
		ID3D11Resource* res = nullptr;
		m_textures[texID]->GetResource(&res);

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = res->QueryInterface(&texture);

		Vec2 dim(0, 0);
		if(SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			dim.x = static_cast<float>(desc.Width);
			dim.y = static_cast<float>(desc.Height);
		}
		safeRelease(texture);
		safeRelease(res);
		return dim;
	}	
	
	//*****************************************************************
	//					DRAW LINE
	//*****************************************************************
	void GraphicsSystem::drawLine(const Transform& tf, const Vec2* pos, uint32_t count, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		std::vector<Vertex> vertices(count);
		for(uint32_t i = 0; i < count; ++i)
		{
			vertices[i].setPosition(pos[i].x, pos[i].y, 0);
			vertices[i].setDiffuse(1, 1, 1, 1);
		}

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), count * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		/****** CONSTANT BUFFER ******/
		auto* cb = makeConstantBuffer(m_dev, sizeof(cbPerObject));

		m_world = XMMatrixIdentity();
		m_world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		m_world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		m_world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);


		cbPerObject cbpo;
		cbpo.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);
		cbpo.FillColor.x = c.r;
		cbpo.FillColor.y = c.g;
		cbpo.FillColor.z = c.b;
		cbpo.FillColor.w = c.a;
		cbpo.isTexture.x = 0;

		m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
		m_devcon->VSSetConstantBuffers(0, 1, &cb);
		m_devcon->PSSetConstantBuffers(0, 1, &cb);

		m_devcon->Draw(count, 0);

		cb->Release();
		vb->Release();
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
		}

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices.data(), count * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		/****** CONSTANT BUFFER ******/
		auto* cb = makeConstantBuffer(m_dev, sizeof(cbPerObject));

		m_world = XMMatrixIdentity();
		m_world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		m_world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		m_world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);


		cbPerObject cbpo;
		cbpo.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);
		cbpo.FillColor.x = c.r;
		cbpo.FillColor.y = c.g;
		cbpo.FillColor.z = c.b;
		cbpo.FillColor.w = c.a;
		cbpo.isTexture.x = 0;

		m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
		m_devcon->VSSetConstantBuffers(0, 1, &cb);
		m_devcon->PSSetConstantBuffers(0, 1, &cb);

		m_devcon->Draw(count, 0);

		cb->Release();
		vb->Release();
	}

	//*****************************************************************
	//					DRAW QUAD
	//*****************************************************************
	void GraphicsSystem::drawQuad(const Transform& tf, const Vec2& hs, const Color& c)
	{
		D3D11_MAPPED_SUBRESOURCE ms;

		/****** VERTEX BUFFER ******/
		Vertex vertices[]
		{
			Vertex(-hs.x, hs.y, 0, 1, 1, 1, 1, 0, 0),
			Vertex( hs.x, hs.y, 0, 1, 1, 1, 1, 0, 0),
			Vertex(-hs.x,-hs.y, 0, 1, 1, 1, 1, 0, 0),
			Vertex( hs.x,-hs.y, 0, 1, 1, 1, 1, 0, 0),
		};

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);
		
		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices, 4*sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		/****** INDEX BUFER ******/
		uint32_t indices[]
		{
			0,1,2,
			1,3,2,
		};

		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), 6);
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices, 6*sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** CONSTANT BUFFER ******/
		auto* cb = makeConstantBuffer(m_dev, sizeof(cbPerObject));

		m_world = XMMatrixIdentity();
		m_world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		m_world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		m_world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);
		

		cbPerObject cbpo;
		cbpo.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);
		cbpo.FillColor.x = c.r;
		cbpo.FillColor.y = c.g;
		cbpo.FillColor.z = c.b;
		cbpo.FillColor.w = c.a;
		cbpo.isTexture.x = 0;

		m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
		m_devcon->VSSetConstantBuffers(0, 1, &cb);
		m_devcon->PSSetConstantBuffers(0, 1, &cb);

		//m_devcon->Draw(4, 0);
		m_devcon->DrawIndexed(6, 0, 0);

		cb->Release();
		ib->Release();
		vb->Release();
	}

	//*****************************************************************
	//					DRAW TEXTURED QUAD
	//*****************************************************************
	void GraphicsSystem::drawTexturedQuad(const Transform& tf, const Color& c, const Image& img, uint32_t tx)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		
		/****** VERTEX BUFFER ******/
		Vertex vertices[]
		{
			Vertex(-img.m_ratio,  1, 0, 1, 1, 1, 1, img.m_texCoords[0].x, img.m_texCoords[0].y),
			Vertex( img.m_ratio,  1, 0, 1, 1, 1, 1, img.m_texCoords[1].x, img.m_texCoords[1].y),
			Vertex(-img.m_ratio, -1, 0, 1, 1, 1, 1, img.m_texCoords[3].x, img.m_texCoords[3].y),
			Vertex( img.m_ratio, -1, 0, 1, 1, 1, 1, img.m_texCoords[2].x, img.m_texCoords[2].y)
		};

		auto* vb = makeVertexBuffer(m_dev, sizeof(Vertex), 4);

		HRESULT hr = m_devcon->Map(vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, vertices, 4 * sizeof(Vertex));
		m_devcon->Unmap(vb, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		/****** INDEX BUFER ******/
		uint32_t indices[]
		{
			0, 1, 2,   1, 3, 2
		};

		auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), 6);
		hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		assert(SUCCEEDED(hr));
		memcpy(ms.pData, indices, 6 * sizeof(uint32_t));
		m_devcon->Unmap(ib, 0);

		m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		/****** CONSTANT BUFFER ******/
		auto* cb = makeConstantBuffer(m_dev, sizeof(cbPerObject));

		m_world = XMMatrixIdentity();
		m_world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
		//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
		//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
		m_world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));
		m_world *= XMMatrixTranslation(tf.position.x, tf.position.y, 0);


		cbPerObject cbpo;
		cbpo.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);
		cbpo.FillColor.x = c.r;
		cbpo.FillColor.y = c.g;
		cbpo.FillColor.z = c.b;
		cbpo.FillColor.w = c.a;
		cbpo.isTexture.x = 1;

		m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
		m_devcon->VSSetConstantBuffers(0, 1, &cb);
		m_devcon->PSSetConstantBuffers(0, 1, &cb);
		m_devcon->PSSetSamplers(0, 1, &m_samplerState);
		auto srv = m_textures[tx].get();
		m_devcon->PSSetShaderResources(0, 1, &srv);

		//m_devcon->Draw(4, 0);
		m_devcon->DrawIndexed(6, 0, 0);

		cb->Release();
		ib->Release();
		vb->Release();
	}
	
	//*****************************************************************
	//					DRAW TEXT
	//*****************************************************************
	void GraphicsSystem::drawText(const std::string& text, const Transform& tf, const Color& tint, uint32_t justification, bool isItalic)
	{
		ID3D11Resource* res = nullptr;
		m_textures[m_fontTextureID]->GetResource(&res);

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = res->QueryInterface(&texture);

		if(SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			float w = static_cast<float>(desc.Width);
			float h = static_cast<float>(desc.Height);
			std::vector<Vertex> verts(text.size() * 4);
			std::vector<uint32_t> inds(text.size() * 6);
			uint32_t v = 0;
			uint32_t i = 0;
			float xPos = 0;
			float yPos = m_font.m_size * 0.5f;
			
			for(char c : text)
			{
				Glyph& glyph = m_font.m_glyphs[c - 32];
				float tv_top = (float)glyph.m_top / h;
				float tv_bot = (float)(glyph.m_top + m_font.m_size) / h;
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
				verts[v].setDiffuse(1,1,1,1);
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

			uint32_t stride = sizeof(Vertex);
			uint32_t offset = 0;
			m_devcon->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
			m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			/****** INDEX BUFER ******/
			auto* ib = makeIndexBuffer(m_dev, sizeof(uint32_t), inds.size());
			hr = m_devcon->Map(ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			assert(SUCCEEDED(hr));
			memcpy(ms.pData, inds.data(), inds.size() * sizeof(uint32_t));
			m_devcon->Unmap(ib, 0);

			m_devcon->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

			/****** CONSTANT BUFFER ******/
			auto* cb = makeConstantBuffer(m_dev, sizeof(cbPerObject));

			m_world = XMMatrixIdentity();
			m_world *= XMMatrixScaling(tf.scale.x, tf.scale.y, 1.0f);
			//m_world *= XMMatrixRotationX(XMConvertToRadians(rotationX));
			//m_world *= XMMatrixRotationY(XMConvertToRadians(rotationY));
			m_world *= XMMatrixRotationZ(XMConvertToRadians(tf.rotation));

			float move = 0;
			if (justification == 1)
			{
				move = xPos*0.5f*tf.scale.x;
			}
			else if (justification == 2)
			{
				move = xPos*tf.scale.x;
			}

			m_world *= XMMatrixTranslation(tf.position.x - move, tf.position.y, 0);


			cbPerObject cbpo;
			cbpo.WVP = XMMatrixTranspose(m_world * /*m_camView * */m_camProjection);
			cbpo.FillColor.x = tint.r;
			cbpo.FillColor.y = tint.g;
			cbpo.FillColor.z = tint.b;
			cbpo.FillColor.w = tint.a;
			cbpo.isTexture.x = 1;

			m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
			m_devcon->VSSetConstantBuffers(0, 1, &cb);
			m_devcon->PSSetConstantBuffers(0, 1, &cb);
			m_devcon->PSSetSamplers(0, 1, &m_samplerState);
			auto srv = m_textures[m_fontTextureID].get();
			m_devcon->PSSetShaderResources(0, 1, &srv);

			m_devcon->DrawIndexed(inds.size(), 0, 0);



			safeRelease(cb);
			safeRelease(ib);
			safeRelease(vb);
		}

		safeRelease(res);
		safeRelease(texture);
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

				DEBUG_CODE_START
					flags |= D3D11_CREATE_DEVICE_DEBUG;
				DEBUG_CODE_END

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
	bool GraphicsSystem::initVertexShader(const char* shaderFile)
	{
		ID3D10Blob* m_shaderBlob = nullptr;
		
		HRESULT hr = D3DX11CompileFromFile(shaderFile, nullptr, nullptr, "VShader", "vs_4_0", 0, 0, nullptr, &m_shaderBlob, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			hr = m_dev->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, &m_vertexShader);
			
			if(SUCCEEDED(hr))
			{
				auto& ied = Vertex::getDescription();

				hr = m_dev->CreateInputLayout(ied.data(), ied.size(), m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), &m_inputLayout);

				if(SUCCEEDED(hr))
				{
					m_devcon->VSSetShader(m_vertexShader, nullptr, 0);
					m_devcon->IASetInputLayout(m_inputLayout);
				}
			}
			m_shaderBlob->Release();
		}
		DEBUG_CODE_START
			if(FAILED(hr))
				DEBUG_INFO("\tinitVertexShader failed");
		DEBUG_CODE_END
		return SUCCEEDED(hr);
	}
	
	//*****************************************************************
	//					INIT PIXEL SHADER
	//*****************************************************************
	bool GraphicsSystem::initPixelShader(const char* shaderFile)
	{
		ID3D10Blob* m_shaderBlob = nullptr;

		HRESULT hr = D3DX11CompileFromFile(shaderFile, nullptr, nullptr, "PShader", "ps_4_0", 0, 0, nullptr, &m_shaderBlob, nullptr, nullptr);
		if(SUCCEEDED(hr))
		{
			hr = m_dev->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, &m_pixelShader);
			if(SUCCEEDED(hr))
			{
				m_devcon->PSSetShader(m_pixelShader, nullptr, 0);
			}
			m_shaderBlob->Release();
		}
		DEBUG_CODE_START
			if(FAILED(hr))
				DEBUG_INFO("\tinitPixelShader failed");
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
	//					INIT FONT
	//*****************************************************************
	bool GraphicsSystem::initFont(DataFile& file)
	{
		m_font.m_name = file.getString("name", "");
		m_font.m_texture = file.getString("texture", "");
		m_font.m_size = file.getInt("size", 0);
		bool success = false;
		if(!m_font.m_name.empty() && !m_font.m_texture.empty() && m_font.m_size > 0)
		{
			uint32_t glyphCount = file.getListSize("glyphs");
			m_font.m_glyphs.resize(glyphCount);
			if(file.getList("glyphs"))
			{
				for(uint32_t i = 0; i < glyphCount; ++i)
				{
					if(file.getList(i + 1))
					{
						auto ascii = file.getInt("ascii", 0);
						auto left = file.getInt("left", -1);
						auto right = file.getInt("right", -1);
						auto top = file.getInt("top", -1);
						if(ascii != 0 && left != -1 && right != -1 && top != -1)
						{
							m_font.m_glyphs[i].m_ascii = ascii;
							m_font.m_glyphs[i].m_character = static_cast<char>(m_font.m_glyphs[i].m_ascii);
							m_font.m_glyphs[i].m_left = left;
							m_font.m_glyphs[i].m_right = right;
							m_font.m_glyphs[i].m_top = top;
						}
						else
						{
							DEBUG_INFO("Glyph in ", file.getFilename(), " font sheet is invalid: ", ascii, ",", left, ",", right, ",", top);
						}
						file.popList();
					}
				}
				file.popList();
			}

			success = !m_font.m_glyphs.empty();
		}
		m_fontTextureID = loadTextureFromFile(m_font.m_texture.c_str());
		return m_fontTextureID != -1;
	}

	//*****************************************************************
	//					LOAD TEXTURE FROM FILE
	//*****************************************************************
	uint32_t GraphicsSystem::loadTextureFromFile(const char* filename)
	{
		assert(filename != nullptr);
		ID3D11ShaderResourceView* texturePtr = nullptr;
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, ResourcePath(filename).c_str(), nullptr, nullptr, &texturePtr, nullptr);
		uint32_t id = -1;
		if(SUCCEEDED(hr))
		{
			m_textures.emplace_back(DxTexturePtr(texturePtr, releasePtr<ID3D11ShaderResourceView>));
			id = m_textures.size() - 1;
		}
		return id;
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
}