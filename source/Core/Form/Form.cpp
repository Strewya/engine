//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Form/Form.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Engine/ResourceLocator.h"
#include "Engine/ServiceLocator.h"
#include "Subsystems/Graphics/IRenderer.h"
#include "Subsystems/Graphics/SpritesheetCache.h"
#include "Subsystems/Graphics/ITextureCache.h"
	/*** end headers ***/

namespace Core
{
	void DrawForms(const std::deque<Form*>& frms, const Core::ServiceLocator& services, const Core::ResourceLocator& resources)
	{
		Graphics::IRenderer& gfx = services.getGraphics();
		for(Form* f : frms)
		{
			if(!f->getVisibility())
			{
				continue;
			}
			switch(f->getType())
			{
			case FormType::Font:
			{
				StateRptr s_text = f->getState("Text");
				StateRptr s_font = f->getState("Font");
				StateRptr s_area = f->getState("Area");
				if(s_text && s_font)
				{
					/*
					auto& text = s_text->cast<String>();
					auto font = s_font->cast<uint32_t>();
					auto& bounds = s_area ? s_area->as<Util::Rect>() : Util::Rect(f->getPosition(),1,1);
					//read these from states!
					gfx.setFontStyle(false, false, false, false, false, false);
					gfx.DrawFont(font, text.c_str(), &bounds);
					*/
				}
			}
			break;
			
			case FormType::Sprite:
			{
				StateRptr s_currentSprite = f->getState("CurrentSprite");
				StateRptr s_spritesheet = f->getState("Spritesheet");
				if(s_currentSprite && s_spritesheet)
				{
					/*
					uint32_t hSheet = s_spritesheet->as<uint32_t>();
					auto currentFrame = s_currentSprite->as<uint32_t>();
					auto* sheet = resources.getSpritesheetCache().getSpritesheet(hSheet);
					Graphics::TextureData* texture = resources.getTextureCache().getTexture(sheet->getTextureHandle());
					gfx.setTransform2D(&f->getPosition(), &f->getScalingCenter(), &f->getScale(), &f->getPivotPoint(), Deg2Rad(f->getRotation()), &f->getColor());
					gfx.DrawSprite(*texture, sheet->getSprite(currentFrame));
					*/
				}
			}
			break;
			
			case FormType::Texture:
			{
				StateRptr s_texture = f->getState("Texture");
				if(s_texture)
				{
					/*
					uint32_t hTexture = s_texture->as<uint32_t>();
					auto* texture = resources.getTextureCache().getTexture(hTexture);
					gfx.setTransform2D(&f->getPosition(), &f->getScalingCenter(), &f->getScale(), &f->getPivotPoint(), Deg2Rad(f->getRotation()), &f->getColor());
					gfx.DrawTexture(*texture);
					*/
				}
			}
			break;

			case FormType::Line:
			{
				StateRptr s_line = f->getState("LineVector");
				StateRptr s_width = f->getState("LineWidth");
				if(s_line)
				{
					/*
					auto& line = s_line->as<Util::Vec2>();
					auto lineWidth = s_width ? s_width->as<float>() : 1.0f;
					gfx.DrawLine(f->getPosition(), f->getPosition()+line, &f->getColor(), lineWidth);
					*/
				}
			}
			break;

			case FormType::Triangle:
			{
				StateRptr s_pt1 = f->getState("Point1");
				StateRptr s_pt2 = f->getState("Point2");
				StateRptr s_pt3 = f->getState("Point3");
				StateRptr s_width = f->getState("LineWidth");
				if(s_pt1 && s_pt2 && s_pt3)
				{
					/*
					Util::Vec2 pts[] = { s_pt1->as<Util::Vec2>(), s_pt2->as<Util::Vec2>(), s_pt3->as<Util::Vec2>()};
					auto lineWidth = s_width ? s_width->as<float>() : 1.0f;
					gfx.DrawTriangle(f->getPosition()+pts[0], f->getPosition()+pts[1], f->getPosition()+pts[2], &f->getColor(), lineWidth);
					*/
				}
			}
			break;

			case FormType::Rectangle:
			{
				StateRptr s_area = f->getState("Area");
				StateRptr s_width = f->getState("LineWidth");
				if(s_area)
				{
					/*
					auto& bounds = s_area->as<Util::Rect>();
					auto lineWidth = s_width ? s_width->as<float>() : 1.0f;
					gfx.DrawRectangle(bounds, &f->getColor(), lineWidth);
					*/
				}
			}
			break;

			case FormType::Circle:
			{
				StateRptr s_radius = f->getState("Radius");
				StateRptr s_width = f->getState("LineWidth");
				if(s_radius)
				{
					/*
					auto radius = s_radius->as<float>();
					auto lineWidth = s_width ? s_width->as<float>() : 1.0f;
					gfx.DrawCircle(f->getPosition(), radius, &f->getColor(), lineWidth);
					*/
				}
			}
			break;

			case FormType::Elipse:
			{
				StateRptr s_radiusX = f->getState("RadiusX");
				StateRptr s_radiusY = f->getState("RadiusY");
				StateRptr s_width = f->getState("LineWidth");
				if(s_radiusX && s_radiusY)
				{
					/*
					float radius[] = {s_radiusX->as<float>(), s_radiusY->as<float>()};
					auto lineWidth = s_width ? s_width->as<float>() : 1.0f;
					gfx.DrawElipse(f->getPosition(), radius[0], radius[1],&f->getColor(), lineWidth);
					*/
				}
			}
			break;
			}
		}
	}
	

	Form::Form()
		: _type(FormType::Null), _scale(1,1), _rotation(0), _position(0,0), _states(),
		_color(255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true), _space(0)
	{
	}

	Form::Form(const FormType& type)
		: _type(type), _scale(1,1), _rotation(0), _position(0,0), _states(),
		_color(255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true), _space(0)
	{
	}

	/********** GETTERS *****************/
	Space& Form::getSpace()
	{
		return _space;
	}

	FormType Form::getType() const
	{
		return _type;
	}
	
	float Form::getRotation() const
	{
		return _rotation;
	}

	bool Form::getVisibility() const
	{
		return _visible;
	}

	const Util::Color& Form::getColor() const
	{
		return _color;
	}

	const Util::Vec2& Form::getPosition() const
	{
		return _position;
	}

	const Util::Vec2& Form::getScalingCenter() const
	{
		return _scalingCenter;
	}

	const Util::Vec2& Form::getScale() const
	{
		return _scale;
	}

	const Util::Vec2& Form::getPivotPoint() const
	{
		return _pivotPoint;
	}

	/************************ SETTERS ****************************/
	void Form::setType(const FormType& type)
	{
		_type = type;
	}
	
	void Form::setColor(const Util::Color& color)
	{
		_color = color;
	}

	void Form::setColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		_color.setChannels(r, g, b, a);
	}
	
	void Form::setPosition(const Util::Vec2& position)
	{
		_position = position;
	}

	void Form::setPosition(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}

	void Form::setScalingCenter(const Util::Vec2& scaleCenter)
	{
		_scalingCenter = scaleCenter;
	}

	void Form::setScalingCenter(float x, float y)
	{
		_scalingCenter.x = x;
		_scalingCenter.y = y;
	}

	void Form::setScale(const Util::Vec2& scale)
	{
		_scale = scale;
	}

	void Form::setScale(float x, float y)
	{
		_scale.x = x;
		_scale.y = y;
	}

	void Form::setPivotPoint(const Util::Vec2& pivot)
	{
		_pivotPoint = pivot;
	}
	
	void Form::setPivotPoint(float x, float y)
	{
		_pivotPoint.x = x;
		_pivotPoint.y = y;
	}

	void Form::setRotation(const float rotation)
	{
		int sign = (rotation < 0) ? (-1) : (1);
		_rotation = std::abs(rotation) >= 360 ? rotation-(sign*360) : rotation;
	}

	void Form::setVisibility(bool state)
	{
		_visible = state;
	}

	void Form::Scale(const Util::Vec2& scaling)
	{
		_scale += scaling;
	}

	void Form::Scale(float x, float y)
	{
		_scale.x += x;
		_scale.y += y;
	}
	
	void Form::Rotate(float rotation)
	{
		_rotation += rotation;
	}
	
	void Form::Translate(const Util::Vec2& translation)
	{
		_position += translation;
	}

	void Form::Translate(float x, float y)
	{
		_position.x += x;
		_position.y += y;
	}

	bool Form::Insert(const char* name, std::unique_ptr<BaseState> state)
	{
		return _states.insert(name, std::move(state));
	}

	bool Form::Insert(const String& name, std::unique_ptr<BaseState> state)
	{
		return _states.insert(name, std::move(state));
	}

	void Form::ClearStates()
	{
		_states.clear();
	}

	bool Form::RemoveState(const char* name)
	{
		return _states.destroy(name);
	}

	bool Form::RemoveState(const String& name)
	{
		return _states.destroy(name);
	}

	BaseState* Form::getState(const char* name)
	{
		return _states.retrieve(name);
	}

	BaseState* Form::getState(const String& name)
	{
		return _states.retrieve(name);
	}

	bool Form::hasState(const char* name, bool recursive)
	{
		return _states.contains(name);
	}

	bool Form::hasState(const String& name, bool recursive)
	{
		return _states.contains(name);
	}
}