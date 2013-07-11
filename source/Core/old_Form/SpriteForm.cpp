//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Form/SpriteForm.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/SpriteInfo.h"
#include "Subsystems/Graphics/Spritesheet.h"
	/*** end headers ***/

namespace Core
{
	const String Sprite::_type = "Sprite";
	
	Sprite::Sprite()
		: _sheet(nullptr), _currentSprite(0)
	{
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::DrawSelf(Graphics::Interface& gfx)
	{
		if(_sheet == nullptr)
		{
			Util::GetDefaultLogger() << "Spritesheet is nullptr in SpriteForm " << _id << "!" << Util::Logger::endl;
			return;
		}
		if(!_sheet->Valid(_currentSprite))
		{
			Util::GetDefaultLogger() << "Sprite index " << _currentSprite << " is not valid!" << Util::Logger::endl;
			return;
		}
		if(_visible)
		{
			//before this call, coordinates should be changed from world to screen
			gfx.setTransform2D(&_position, &_scalingCenter, &_scale, &_pivotPoint, _rotation, &_color);
			gfx.DrawSprite(_sheet->getTextureHandle(), _sheet->getSprite(_currentSprite));
		}
	}

	void Sprite::AnimateSelf()
	{
		//do some checks whether the sheet or sprite should be changed
	}

	const String& Sprite::getType() const
	{
		return _type;
	}

	void Sprite::setSpritesheet(Graphics::Spritesheet& sheet)
	{
		_sheet = &sheet;
	}

	void Sprite::setSpriteInfo(uint sprite)
	{
		_currentSprite = sprite;
	}

	/*
	Mesh2DForm::Mesh2DForm(Entity& owner, const String& texture, const String& image)
		: Form(owner), _texture(getServices().getGraphics().GetTexture(texture)), _image()
	{
		assert(_texture.get());
		_image = _texture->GetImage(image);
	}

	Mesh2DForm::~Mesh2DForm()
	{
		_image = NULL;
	}

	void Mesh2DForm::OnDrawSelf()
	{
		IGraphics& graphics = getServices().getGraphics();
		if(!getParent().getStates().Exists("DEBUG.NOTEX") || !(*sBool::Data(getParent().getStates().Get("DEBUG.NOTEX"))))
		{
			graphics.DrawImage(_image, _texture);
		}

		if(getParent().getStates().Exists("DEBUG.COLMASK") && *sBool::Data(getParent().getStates().Get("DEBUG.COLMASK")))
		{
			if(getStates().Exists("CollisionMask"))
			{
				State* mask = getStates().Get("CollisionMask");
				if(mask->getType() == "rect")
				{
					Rect r = sRect::Convert(mask)->value;
					r.position = getPosition();
					r.width *= getScale().x;
					r.height *= getScale().y;
					graphics.SetColor(Color(0,255,0));
					graphics.SetLineWidth(1);
					graphics.DrawRectangle(r);
				}
			}
			
			graphics.DrawCircle(getPosition(), 1);
		}
	}

	void Mesh2DForm::OnAnimateSelf()
	{
		sString* animation = sString::Convert(getStates().Get("CurrentAnimation"));
		sTime* time = sTime::Convert(getStates().Get("AnimationTime"));
		sFloat* speed = sFloat::Convert(getStates().Get("AnimationSpeed"));
		sUint* frame = sUint::Convert(getStates().Get("AnimationFrame"));

		Animation* anim = _texture->GetAnimation(**animation);
		if(anim == NULL)
		{
			return;
		}
		
		uint frameCount = anim->sequence.size();
		TIME frameTime = (TIME) **speed/frameCount;

		if(frameCount > 1 && **time >= frameTime)
		{
			**time -= frameTime;
			switch(anim->mode)
			{
			case SG::Repeat_Loop:
				++**frame;
				**frame %= frameCount;
				break;
			case SG::Repeat_Once:
				if(**frame < frameCount-1)
					++**frame;
				break;
			}
			_image = anim->GetFrame(**frame);
		}
	}

	const String& Mesh2DForm::getType() const
	{
		return _type;
	}

	Image& Mesh2DForm::getImage()
	{
		return *_image;
	}

	shared_ptr<Texture> Mesh2DForm::getTexture()
	{
		return _texture;
	}

	void Mesh2DForm::setImage(Image& img)
	{
		_image = &img;
	}

	void Mesh2DForm::setTexture(shared_ptr<Texture> tex)
	{
		assert(tex.get());
		_texture = tex;
	}
	*/
}