//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/GraphicsSystem.h>
/******* C++ headers *******/
#include <cassert>
#include <iostream>
/******* extra headers *******/
#include <Graphics/Vertex.h>
#include <Scripting/ScriptingSystem.h>
#include <Util/Color.h>
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
		declare(&m_fontTexture);

		m_window = &window;
		m_backgroundColor.r = m_backgroundColor.g = m_backgroundColor.b = 0;
		
		status = initDevice() &&
			initSwapChain() &&
			initRenderTarget() &&
			initViewport();

		m_camPosition = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		m_camLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_camView = XMMatrixLookAtLH(m_camPosition, m_camLookAt, m_camUp);
		//m_camProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), (float)m_window->getSizeX() / m_window->getSizeY(), 1.0f, 100.0f);
		m_camProjection = XMMatrixOrthographicLH((float)m_window->getSizeX(), (float)m_window->getSizeY(), 1.0f, 100.0f);
		
		DEBUG_INFO("GraphicsSystem init ", status ? "OK" : "FAIL");
		return status;
	}

	bool GraphicsSystem::shutdown()
	{
		bool status = true;

		if(m_swapchain != nullptr)
		{
			m_swapchain->SetFullscreenState(false, nullptr);
		}

		for(auto* ptr : m_dxResources)
		{
			safeRelease(ptr);
		}

		DEBUG_INFO("GraphicsSystem shutdown ", status ? "OK" : "FAIL");
		return status;
	}

	void GraphicsSystem::update()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);

		//go through the command list and execute each one
		//each command is a special draw call for whatever needs to be drawn

		m_swapchain->Present(0, 0);
	}

	void GraphicsSystem::begin()
	{
		m_devcon->ClearRenderTargetView(m_renderTarget, m_backgroundColor);
	}

	void GraphicsSystem::present()
	{
		m_swapchain->Present(0, 0);
	}

	void GraphicsSystem::setBackgroundColor(float r, float g, float b)
	{
		m_backgroundColor.r = r;
		m_backgroundColor.g = g;
		m_backgroundColor.b = b;
	}

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
	
	void GraphicsSystem::drawText(const std::string& text, const Transform& tf, const Color& tint, uint32_t justification)
	{
		ID3D11Resource* res = nullptr;
		m_fontTexture->GetResource(&res);

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

				verts[v].setPosition(xPos, -yPos, 0);
				verts[v].setDiffuse(1,1,1,1);
				verts[v].setTextureCoords(tu_left, tv_bot);
				++v;
				verts[v].setPosition(xPos, yPos, 0);
				verts[v].setDiffuse(1, 1, 1, 1);
				verts[v].setTextureCoords(tu_left, tv_top);
				++v;
				xPos += (glyph.m_right - glyph.m_left);
				verts[v].setPosition(xPos, yPos, 0);
				verts[v].setDiffuse(1, 1, 1, 1);
				verts[v].setTextureCoords(tu_rght, tv_top);
				++v;
				verts[v].setPosition(xPos, -yPos, 0);
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
			cbpo.WVP = XMMatrixTranspose(m_world * m_camView * m_camProjection);
			cbpo.FillColor.x = tint.r;
			cbpo.FillColor.y = tint.g;
			cbpo.FillColor.z = tint.b;
			cbpo.FillColor.w = tint.a;
			cbpo.isTexture.x = 1;

			m_devcon->UpdateSubresource(cb, 0, nullptr, &cbpo, 0, 0);
			m_devcon->VSSetConstantBuffers(0, 1, &cb);
			m_devcon->PSSetConstantBuffers(0, 1, &cb);
			m_devcon->PSSetSamplers(0, 1, &m_samplerState);
			m_devcon->PSSetShaderResources(0, 1, &m_fontTexture);

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
#ifdef _DEBUG
				flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
				hr = D3D11CreateDevice(adapter, driverType, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_dev, nullptr, &m_devcon);

				adapter->Release();
			}
		}

		DEBUG_LINE(if(FAILED(hr)) DEBUG_INFO("\tinitDevice failed"));

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
				m_devcon->OMSetRenderTargets(1, &m_renderTarget, nullptr);
			}
			bbTexture->Release();
		}
		DEBUG_LINE(if (FAILED(hr)) DEBUG_INFO("\tinitRenderTarget failed"));
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
		DEBUG_LINE(if (FAILED(hr)) DEBUG_INFO("\tinitVertexShader failed"));
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
		DEBUG_LINE(if (FAILED(hr)) DEBUG_INFO("\tinitPixelShader failed"));
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
	//					INIT FONT
	//*****************************************************************
	bool GraphicsSystem::initFont(DataFile& file)
	{
		m_font.m_name = file.getString("name");
		m_font.m_texture = file.getString("texture");
		m_font.m_size = file.getInt("size");
		uint32_t glyphCount = file.getInt("glyphCount");
		m_font.m_glyphs.resize(glyphCount);
		for(uint32_t i = 0; i < glyphCount; ++i)
		{
			m_font.m_glyphs[i].m_ascii = file.getInt(("glyphs[" + std::to_string(i + 1) + "].ascii").c_str());
			m_font.m_glyphs[i].m_character = static_cast<char>(m_font.m_glyphs[i].m_ascii);
			m_font.m_glyphs[i].m_left = file.getInt(("glyphs[" + std::to_string(i + 1) + "].left").c_str())-1;
			m_font.m_glyphs[i].m_right = file.getInt(("glyphs[" + std::to_string(i + 1) + "].right").c_str())+1;
			m_font.m_glyphs[i].m_top = file.getInt(("glyphs[" + std::to_string(i + 1) + "].top").c_str());
		}

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_dev, RESOURCE_S(m_font.m_texture), nullptr, nullptr, &m_fontTexture, nullptr);
		return SUCCEEDED(hr);
	}

	

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