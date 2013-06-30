//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Subsystems/Graphics/DirectX/DXRenderer.h"
	/*** C++ headers ***/
#include <algorithm>
	/*** extra headers ***/
#include "Subsystems/Graphics/TextureData.h"
#include "Util/Logger.h"
	/*** end headers ***/

namespace Graphics
{
	DXRenderer::DXRenderer(HWND hwnd)
		: _transparentColor(D3DCOLOR_XRGB(255,0,255)), _backgroundFillColor(D3DCOLOR_XRGB(0,0,0)), _hwnd(hwnd), _d3d(nullptr), _d3ddev(nullptr), _backbuffer(nullptr),
		_spriteHandler(nullptr), _line(nullptr)
	{
		if(!InitDevices())
		{
			throw std::exception("DirectX graphics subsystem has failed to load.");
		}

		_textures.SetD3DDevice(_d3ddev);
	}

	DXRenderer::~DXRenderer()
	{
		_textures.Clear();

		if(_line != nullptr)
		{
			_line->Release();
			_line = nullptr;
		}

		if(_spriteHandler != nullptr)
		{
			_spriteHandler->Release();
			_spriteHandler = nullptr;
		}

		if(_backbuffer != nullptr)
		{
			_backbuffer->Release();
			_backbuffer = nullptr;
		}

		if(_d3ddev != nullptr)
		{
			_d3ddev->Release();
			_d3ddev = nullptr;
		}
		
		if(_d3d != nullptr)
		{
			_d3d->Release();
			_d3d = nullptr;
		}
	}

	bool DXRenderer::InitDevices()
	{
		_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(_d3d==nullptr)
		{
			return false;
		}
		//set Direct3D presentation parameters
		ZeroMemory(&_d3dpp, sizeof(_d3dpp));
		_d3dpp.Windowed					= true;//(!_settings.IsFullscreen());
		_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		_d3dpp.BackBufferFormat			= D3DFMT_R5G6B5; //D3DFMT_X8R8G8B8;
		_d3dpp.BackBufferCount			= 1;
		_d3dpp.BackBufferWidth			= 1024;//_settings.GetWidth();
		_d3dpp.BackBufferHeight			= 768;//_settings.GetHeight();
		_d3dpp.hDeviceWindow			= _hwnd;
		_d3dpp.EnableAutoDepthStencil	= false; //true;
		_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT; //_IMMEDIATE
		_d3dpp.MultiSampleQuality		= 0;
		_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
		_d3dpp.Flags					= 0;
		//create Direct3D device
		if(FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_d3dpp, &_d3ddev)))
		{
			return false;
		}
		if(_d3ddev == nullptr)
			return false;
		//enable the depth buffer
		if(FAILED(_d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE)))
			return false;
		//clear the backbuffer to black
		if(FAILED(_d3ddev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0)))
			return false;
		//create pointer to backbuffer
		if(FAILED(_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backbuffer)))
			return false;
		//create pointer to sprite class
		if(FAILED(D3DXCreateSprite(_d3ddev, &_spriteHandler)))
			return false;
		//create pointer to line class
		if(FAILED(D3DXCreateLine(_d3ddev, &_line)))
			return false;
		//create a default texture to use when another texture fails to load
		//should be embedded into the DLL if possible
		if(FAILED(D3DXCreateTexture(_d3ddev, 1, 1, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &_defaultTexture)))
			return false;

		return true;
	}

	bool DXRenderer::ResetDevices()
	{
		_spriteHandler->OnLostDevice();
		_line->OnLostDevice();
		_backbuffer->Release();
		if(FAILED(_d3ddev->Reset(&_d3dpp)))
			return false;
		if(FAILED(_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backbuffer)))
			return false;
		_spriteHandler->OnResetDevice();
		_line->OnResetDevice();
		uint32_t style, exStyle;
		if(!_d3dpp.Windowed)
		{
			style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
			exStyle = WS_EX_APPWINDOW;
		}
		else
		{
			style = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
			exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}
		RECT rc = {0,0, _d3dpp.BackBufferWidth, _d3dpp.BackBufferHeight };
		AdjustWindowRectEx(&rc, style, false, exStyle);
		SetWindowLong(_d3dpp.hDeviceWindow, GWL_STYLE, style);
		SetWindowLong(_d3dpp.hDeviceWindow, GWL_EXSTYLE, exStyle);
		SetWindowPos(_d3dpp.hDeviceWindow, HWND_NOTOPMOST, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		return true;
	}

	ITextureCache& DXRenderer::getTextureCache()
	{
		return _textures;
	}

	Util::Vec2 DXRenderer::getScreenSize() const
	{
		return Util::Vec2((float)_d3dpp.BackBufferWidth, (float)_d3dpp.BackBufferHeight);
	}

	void DXRenderer::setScreenSize(const Util::Vec2& size)
	{
		setScreenSize((uint32_t)size.x, (uint32_t)size.y);
	}

	void DXRenderer::setScreenSize(uint32_t width, uint32_t height)
	{
		_d3dpp.BackBufferWidth = width;
		_d3dpp.BackBufferHeight = height;
		ResetDevices();
	}

	Util::Color DXRenderer::getTransparentColor() const
	{
		return Util::Color(_transparentColor&0x00ff0000>>16, _transparentColor&0x0000ff00>>8, _transparentColor&0x000000ff, _transparentColor&0xff000000>>24);
	}

	void DXRenderer::setTransparentColor(const Util::Color& color)
	{
		Util::Color c = color;
		c.setAlpha(255);
		_transparentColor = MakeCOLOR(c);
	}

	void DXRenderer::setTransparentColor(uint32_t red, uint32_t green, uint32_t blue)
	{
		setTransparentColor(Util::Color(red, green, blue, 255));
	}

	Util::Color DXRenderer::getBackgroundFillColor() const
	{
		return Util::Color(_backgroundFillColor&0x00ff0000>>16, _backgroundFillColor&0x0000ff00>>8, _backgroundFillColor&0x000000ff, _backgroundFillColor&0xff000000>>24);
	}

	void DXRenderer::setBackgroundFillColor(const Util::Color& color)
	{
		Util::Color c = color;
		c.setAlpha(255);
		_backgroundFillColor = MakeCOLOR(c);
	}

	void DXRenderer::setBackgroundFillColor(uint32_t red, uint32_t green, uint32_t blue)
	{
		setBackgroundFillColor(Util::Color(red, green, blue, 255));
	}

	bool DXRenderer::getFullscreenState() const 
	{
		return !_d3dpp.Windowed;
	}

	void DXRenderer::setFullscreenState(bool state)
	{
		if(_d3dpp.Windowed != (BOOL)state)
		{
			_d3dpp.Windowed = state;
			ResetDevices();
		}
	}

	uint32_t DXRenderer::MakeFont(const char* name, uint32_t size, uint32_t weight, bool italic)
	{
		int index = _fonts.getHandle(name);
		if(index != NOT_FOUND)
		{
			const FontInfo& info = _fonts.getFont(index).info;
			if(info.getName().compare(name) == 0 && info.getItalic() == italic && info.getSize() == size && info.getWeight() == weight)
			{
				return index;
			}
			_fonts.Destroy(index); //or throw exception that font with that name already exists
		}
		return _fonts.Insert(LoadFont(name, size, weight, italic));
	}

	uint32_t DXRenderer::getFontHandle(const char* name)
	{
		return _fonts.getHandle(name);
	}

	const FontInfo& DXRenderer::getFontInfo(uint32_t handle) const
	{
		return _fonts.getFont(handle).info;
	}

	DXFont DXRenderer::LoadFont(const char* name, uint32_t size, uint32_t weight, bool italic) const
	{
		LPD3DXFONT font = nullptr;
		HRESULT result = D3DXCreateFont(_d3ddev, size, 0, weight, 1, italic, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, TEXT(name), &font);

		if(FAILED(result))
		{
			throw std::exception((String("Failed to create font '") + name + "'!").c_str());
		}
		
		return DXFont(name, size, italic, weight, font);
	}


	
	/*
	void DXRenderer::LoadGraphics(const String& resourceFile)
	{
		if(resourceFile.empty())
			return;
		//tiny xml loading
		ticpp::Document doc(resourceFile);
		try
		{
			doc.LoadFile();

			ticpp::Iterator< ticpp::Element > resource;
			for( resource = resource.begin( doc.FirstChildElement() ); resource != resource.end(); ++resource )
			{
				String resourceType;
				resource->GetValue(&resourceType);
				if(resourceType == "File")
				{
					String path;
					resource->GetText(&path);
					LoadGraphics(path);
				}
				else if(resourceType == "TextureResource")
				{
					String filename, alias;
					resource->GetText(&filename);
					resource->GetAttributeOrDefault("alias", &alias, filename);
					LoadTexture(filename, alias);
				}
				else if(resourceType == "FontResource")
				{
					String alias, font;
					bool italic;
					uint32_t size, weight;
					
					resource->GetText(&font);
					resource->GetAttributeOrDefault("alias", &alias, font);
					resource->GetAttribute("weight", &weight);
					resource->GetAttribute("italic", &italic);
					resource->GetAttribute("size", &size);
					LoadFont(font, size, weight, italic, alias);
				}
			}
		}
		catch(ticpp::Exception& e)
		{
			Logger() << e.what() << Logger::endline;
		}
	}
	*/
	bool DXRenderer::BeginScene()
	{
		HRESULT result = _d3ddev->TestCooperativeLevel();
		if(result == D3DERR_DEVICELOST)
			return false;
		if(result == D3DERR_DEVICENOTRESET)
		{
			ResetDevices();
			return false;
		}
		if(SUCCEEDED(result) && SUCCEEDED(_d3ddev->BeginScene()))
		{
			_d3ddev->ColorFill(_backbuffer, nullptr, _backgroundFillColor);
			return true;
		}
		return false;
	}
	
	void DXRenderer::EndScene()
	{
		_d3ddev->EndScene();
		_d3ddev->Present(nullptr, nullptr, nullptr, nullptr);
	}

	bool DXRenderer::BeginSpriteBatch(bool alphablend)
	{
		HRESULT result = _spriteHandler->Begin(alphablend ? D3DXSPRITE_ALPHABLEND : 0);
		return SUCCEEDED(result);
	}

	void DXRenderer::EndSpriteBatch()
	{
		_spriteHandler->End();
	}

	void DXRenderer::setTransform2D(const Util::Vec2* translation, const Util::Vec2* scalingCenter, const Util::Vec2* scale, const Util::Vec2* rotationPivot, float rotationRadians, const Util::Color* colorTint)
	{
		_tintColor = colorTint ? MakeCOLOR(*colorTint) : D3DCOLOR_XRGB(255,255,255);

		D3DXMatrixTransformation2D(&_transformMatrix,
			scalingCenter ? &MakeVECTOR(*scalingCenter) : nullptr,
			0,
			scale ? &MakeVECTOR(*scale) : nullptr,
			rotationPivot ? &MakeVECTOR(*rotationPivot) : nullptr,
			rotationRadians,
			translation ? &MakeVECTOR(*translation) : nullptr);
	}
	
	void DXRenderer::setFontStyle(bool noClip, bool singleLine, bool hCenter, bool right, bool vCenter, bool bottom)
	{
		_fontStyle =	noClip	   ? DT_NOCLIP	: 0 |
						hCenter    ? DT_CENTER	: 0 |
						right	   ? DT_RIGHT	: 0 |
						vCenter    ? DT_VCENTER : 0 |
						bottom	   ? DT_BOTTOM	: 0 |
						singleLine ? DT_SINGLELINE : DT_WORDBREAK;
	}

	void DXRenderer::DrawPoint(const Util::Vec2& pos, const Util::Color* color, float lineWidth)
	{
		D3DXVECTOR2 pts[] = { MakeVECTOR(pos), MakeVECTOR(pos+1) };
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);
		_line->SetWidth(lineWidth);
		_line->Draw(pts, 2, c);
	}

	void DXRenderer::DrawLine(const Util::Vec2& start, const Util::Vec2& finish, const Util::Color* color, float lineWidth)
	{
		D3DXVECTOR2 pts[] = { MakeVECTOR(start), MakeVECTOR(finish) };
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);
		_line->SetWidth(lineWidth);
		_line->Draw(pts, 2, c);
	}

	void DXRenderer::DrawTriangle(const Util::Vec2& p1, const Util::Vec2& p2, const Util::Vec2& p3, const Util::Color* color, float lineWidth)
	{
		D3DXVECTOR2 pts[] = { MakeVECTOR(p1), MakeVECTOR(p2), MakeVECTOR(p3), MakeVECTOR(p1) };
		_line->SetWidth(lineWidth);
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);
		_line->Draw(pts, 4, c);
	}

	void DXRenderer::DrawRectangle(const Util::Vec2& pos, uint32_t hwidth, uint32_t hheight, const Util::Color* color, float lineWidth)
	{
		Util::Vec2 ul = Util::Vec2(float(hwidth), float(hheight));
		Util::Vec2 bl = Util::Vec2(float(hwidth), -float(hheight));
		D3DXVECTOR2 pts[] = { MakeVECTOR(pos - ul), MakeVECTOR(pos + bl),
							MakeVECTOR(pos + ul), MakeVECTOR(pos - bl),
							MakeVECTOR(pos - ul) };
		
		_line->SetWidth(lineWidth);
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);
		_line->Draw(pts, 5, c);

	}

	void DXRenderer::DrawRectangle(const Util::Rect& rect, const Util::Color* color, float lineWidth)
	{
		D3DXVECTOR2 pts[] = {	D3DXVECTOR2(rect.Left(), rect.Top()),
								D3DXVECTOR2(rect.Right(), rect.Top()),
								D3DXVECTOR2(rect.Right(), rect.Bottom()),
								D3DXVECTOR2(rect.Left(), rect.Bottom()),
								D3DXVECTOR2(rect.Left(), rect.Top())
							};
		_line->SetWidth(lineWidth);
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);
		_line->Draw(pts, 5, c);
	}

	void DXRenderer::DrawCircle(const Util::Vec2& pos, float radius, const Util::Color* color, float lineWidth)
	{
		const int numVertex = (int) (max(36*0.25, std::ceil((radius*0.5)*0.25)  ));
		std::vector<D3DXVECTOR2> circle;
		std::vector<D3DXVECTOR2> quarterCircle;
		circle.reserve(numVertex*4 + 1);
		quarterCircle.reserve(numVertex);
		float angle = 360.0f/(numVertex*4);
		for(int i = 0; i < numVertex; ++i)
		{
			quarterCircle.push_back(D3DXVECTOR2((radius*std::cos(D3DXToRadian(angle*i))), (radius*std::sin(D3DXToRadian(angle*i)))));
			circle.push_back(D3DXVECTOR2(pos.x + quarterCircle[i].x, pos.y + quarterCircle[i].y));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x - quarterCircle[i].y, pos.y + quarterCircle[i].x));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x - quarterCircle[i].x, pos.y - quarterCircle[i].y));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x + quarterCircle[i].y, pos.y - quarterCircle[i].x));
		}
		circle.push_back(circle.front());
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);

		_line->SetWidth(lineWidth);
		_line->Draw(&circle[0], numVertex*4+1, c);
	}

	void DXRenderer::DrawElipse(const Util::Vec2& pos, float xRadius, float yRadius, const Util::Color* color, float lineWidth)
	{
		const int numVertex = (int) (max(36*0.25, std::ceil((min(xRadius, yRadius)*0.5)*0.25)  ));
		std::vector<D3DXVECTOR2> circle;
		std::vector<D3DXVECTOR2> quarterCircle;
		circle.reserve(numVertex*4 + 1);
		quarterCircle.reserve(numVertex);
		float angle = 360.0f/(numVertex*4);
		for(int i = 0; i < numVertex; ++i)
		{
			quarterCircle.push_back(D3DXVECTOR2((std::cos(D3DXToRadian(angle*i))), (std::sin(D3DXToRadian(angle*i)))));
			circle.push_back(D3DXVECTOR2(pos.x + xRadius*quarterCircle[i].x, pos.y + yRadius*quarterCircle[i].y));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x - xRadius*quarterCircle[i].y, pos.y + yRadius*quarterCircle[i].x));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x - xRadius*quarterCircle[i].x, pos.y - yRadius*quarterCircle[i].y));
		}
		for(int i = 0; i < numVertex; ++i)
		{
			circle.push_back(D3DXVECTOR2(pos.x + xRadius*quarterCircle[i].y, pos.y - yRadius*quarterCircle[i].x));
		}
		circle.push_back(circle.front());
		D3DXCOLOR c = color == nullptr ? D3DCOLOR_XRGB(255,255,255) : MakeCOLOR(*color);

		_line->SetWidth(lineWidth);
		_line->Draw(&circle[0], numVertex*4+1, c);
	}
	
	void DXRenderer::DrawTexture(const TextureData& texture)
	{
		const DXTexture& dxtexture = static_cast<const DXTexture&>(texture);
		if(dxtexture.lpd3dTexture == nullptr)
		{
			return;
		}
		Util::Rect source(0,0,float(texture.width), float(texture.height));
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &MakeRECT(source), nullptr, nullptr, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}

	void DXRenderer::DrawSprite(const TextureData& texture, const SpriteInfo& sprite)
	{
		const DXTexture& dxtexture = static_cast<const DXTexture&>(texture);
		if(dxtexture.lpd3dTexture == nullptr)
		{
			return;
		}
		Util::Rect source = sprite.getSrcRect();
		_spriteHandler->SetTransform(&_transformMatrix);
		_spriteHandler->Draw(dxtexture.lpd3dTexture, &MakeRECT(source), nullptr, nullptr, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}
	
	void DXRenderer::DrawFont(uint32_t hFont, const char* text, const Util::Rect* bounds)
	{
		if(!_fonts.Valid(hFont))
		{
			//potentionally throw exception, maybe log the error, or maybe write with a default font
			return;
		}
		const DXFont& font = _fonts.getFont(hFont);
		
		_spriteHandler->SetTransform(&_transformMatrix);
		font.data->DrawText(_spriteHandler, text, -1, bounds == nullptr ? nullptr : &MakeRECT(*bounds), _fontStyle, _tintColor);
		_spriteHandler->SetTransform(D3DXMatrixIdentity(&_transformMatrix));
	}

	/*
	bool DXRenderer::LoadFont(const String& font, const uint32_t size, const uint32_t weight, const bool italic, const String& alias)
	{
		String fontAlias = alias.empty() ? font : alias;
		if(!_fonts.Find(fontAlias))
		{
			FontStore::TypePtr fnt;
			if(Create(font, size, weight, italic, fnt))
			{
				_fonts.Insert(fontAlias, fnt);
				return true;
			}
			else
			{
				Logger() << "Failed to load font " << alias+"/"+font << "." << Logger::endline;
			}
		}
		else
		{
			Logger() << "Font " << alias << " already loaded." << Logger::endline;
		}
		return false;
	}

	bool DXRenderer::Create(const String& name, const uint32_t size, const uint32_t weight, const bool italic, FontStore::TypePtr& out)
	{
		shared_ptr<DXFont> font(new DXFont(name));
		HRESULT result = D3DXCreateFont(_d3ddev, size, 0, weight, 1, italic, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, TEXT(name.c_str()), &font->font);

		if(FAILED(result))
		{
			return false;
		}
		
		out = font;
		return true;
	}

	Font::sptr DXRenderer::GetFont(const String& alias)
	{
		FontStore::TypePtr f;
		if(_fonts.Find(alias, f))
			return f;
		return nullptr;
	}
	
	bool DXRenderer::Create(const String& filename, TextureStore::TypePtr& out)
	{
		shared_ptr<DXTexture> texture(new DXTexture(filename));
		
		//the struct for reading bitmap file info
		D3DXIMAGE_INFO info;
		HRESULT result;

		if(texture->source == Source_File)
		{
			//create the new texture by loading a bitmap image file
			result = D3DXGetImageInfoFromFile(texture->filename.c_str(), &info);
			if(FAILED(result))
				return false;
			result = D3DXCreateTextureFromFileEx(
				_d3ddev,					//direct3D device object
				texture->filename.c_str(),	//the image filename
				info.Width,					//the image width
				info.Height,				//the image height
				1,							//mip-map levels (1 for no chain)
				D3DPOOL_DEFAULT,			//the type of surface (default)
				D3DFMT_UNKNOWN,				//texture format
				D3DPOOL_MANAGED,			//memory class for the image
				D3DX_DEFAULT,				//image filter
				D3DX_DEFAULT,				//mip filter
				_transparentColor,			//color key for transparency
				&info,						//bitmap file info (from loaded file)
				nullptr,						//color palette
				&texture->texture);			//destination texture
		}
		else if(texture->source == Source_Resource)
		{
			result = D3DXGetImageInfoFromResource(nullptr, texture->filename.c_str(), &info);
			if(FAILED(result))
				return false;
			result = D3DXCreateTextureFromResourceEx(
				_d3ddev,					//direct3D device object
				nullptr,						//module
				texture->filename.c_str(),	//the image filename
				info.Width,					//the image width
				info.Height,				//the image height
				1,							//mip-map levels (1 for no chain)
				D3DPOOL_DEFAULT,			//the type of surface (default)
				D3DFMT_UNKNOWN,				//texture format
				D3DPOOL_MANAGED,			//memory class for the image
				D3DX_DEFAULT,				//image filter
				D3DX_DEFAULT,				//mip filter
				_transparentColor,			//color key for transparency
				&info,						//bitmap file info (from loaded file)
				nullptr,						//color palette
				&texture->texture);			//destination texture
		}

		texture->width = info.Width;
		texture->height = info.Height;

		if(FAILED(result))
		{
			return false;
		}

		texture->ParseDescriptor();
		out = texture;
		return true;
	}
	
	Texture::sptr DXRenderer::GetTexture(const String& alias)
	{
		TextureStore::TypePtr t;
		if(_textures.Find(alias, t))
			return t;
		return nullptr;
	}

	void DXRenderer::ReloadTexture(const String& alias)
	{
		TextureStore::TypePtr out;
		if(_textures.Find(alias, out))
		{
			out->Release();
			Create(out->filename, out);
		}
	}

	void DXRenderer::ReloadAllTextures()
	{
		TextureStore::Store::iterator it = _textures.data.begin();
		for(; it != _textures.data.end(); ++it)
		{
			it->second->Release();
			Create(it->second->filename, it->second);
		}
	}

	void DXRenderer::ReloadFont(const String& alias)
	{
		FontStore::TypePtr out;
		if(_fonts.Find(alias, out))
		{
			out->Release();
			Create(out->name, out->size, out->weight, out->italic, out);
		}
	}

	void DXRenderer::ReloadAllFonts()
	{
		FontStore::Store::iterator it = _fonts.data.begin();
		for(; it != _fonts.data.end(); ++it)
		{
			it->second->Release();
			Create(it->second->name, it->second->size, it->second->weight, it->second->italic, it->second);
		}
	}
	*/
}