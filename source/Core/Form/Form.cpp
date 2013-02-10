//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Form/Form.h"
	/*** C++ headers ***/
	/*** extra headers ***/
#include "Subsystems/Graphics/Interface.h"
#include "Subsystems/Graphics/Spritesheet.h"
	/*** end headers ***/

namespace Core
{
	void DrawForms(const std::vector<Form*>& frms, Graphics::Interface& gfx)
	{
		for(Form* f : frms)
		{
			switch(f->getType())
			{
			case FormType::Font:
			{
				if(f->getStates().Contains("Text") && f->getStates().Contains("Font"))
				{
					State text = f->getStates().Get("Text");
					State font = f->getStates().Get("Font");
					Util::Rect bounds = f->getStates().Contains("Area") ? f->getStates().Get("Area").as<Util::Rect>() : Util::Rect(f->getPosition(), 1, 1);
					//read these from states!
					gfx.setFontStyle(false, false, false, false, false, false);
					gfx.DrawFont(font.as<uint>(), text.as<String>(), &bounds);
				}
			}
			break;
			
			case FormType::Sprite:
			{
				if(f->getStates().Contains("Spritesheet") && f->getStates().Contains("CurrentSprite"))
				{
					State sheet = f->getStates().Get("Spritesheet");
					State currentFrame = f->getStates().Get("CurrentSprite");
					gfx.setTransform2D(&f->getPosition(), &f->getScalingCenter(), &f->getScale(), &f->getPivotPoint(), Deg2Rad(f->getRotation()), &f->getColor());
					gfx.DrawSprite(sheet.as<Graphics::Spritesheet>().getTextureHandle(), sheet.as<Graphics::Spritesheet>().getSprite(currentFrame.as<uint>()));
				}
			}
			break;
			
			case FormType::Texture:
			{
				if(f->getStates().Contains("Texture"))
				{
					State texture = f->getStates().Get("Texture");
					gfx.setTransform2D(&f->getPosition(), &f->getScalingCenter(), &f->getScale(), &f->getPivotPoint(), Deg2Rad(f->getRotation()), &f->getColor());
					gfx.DrawTexture(texture.as<uint>());
				}
			}
			break;

			case FormType::Line:
			{
				if(f->getStates().Contains("LineVector"))
				{
					State line = f->getStates().Get("LineVector");
					float lineWidth = f->getStates().Contains("LineWidth") ? f->getStates().Get("LineWidth").as<float>() : 1.0f;
					gfx.DrawLine(f->getPosition(), f->getPosition()+line.as<Util::Vec2>(), &f->getColor(), lineWidth);
				}
			}
			break;

			case FormType::Triangle:
			{
				if(f->getStates().Contains("Point1") && f->getStates().Contains("Point2") && f->getStates().Contains("Point3"))
				{
					State pts[] = { f->getStates().Get("Point1"), f->getStates().Get("Point2"), f->getStates().Get("Point3")};
					float lineWidth = f->getStates().Contains("LineWidth") ? f->getStates().Get("LineWidth").as<float>() : 1.0f;
					gfx.DrawTriangle(f->getPosition()+pts[0].as<Util::Vec2>(), f->getPosition()+pts[1].as<Util::Vec2>(), f->getPosition()+pts[2].as<Util::Vec2>(), &f->getColor(), lineWidth);
				}
			}
			break;

			case FormType::Rectangle:
			{
				if(f->getStates().Contains("Area"))
				{
					Util::Rect bounds = f->getStates().Get("Area").as<Util::Rect>();
					float lineWidth = f->getStates().Contains("LineWidth") ? f->getStates().Get("LineWidth").as<float>() : 1.0f;
					gfx.DrawRectangle(bounds, &f->getColor(), lineWidth);
				}
			}
			break;

			case FormType::Circle:
			{
				if(f->getStates().Contains("Radius"))
				{
					State radius = f->getStates().Get("Radius");
					float lineWidth = f->getStates().Contains("LineWidth") ? f->getStates().Get("LineWidth").as<float>() : 1.0f;
					gfx.DrawCircle(f->getPosition(), radius.as<float>(), &f->getColor(), lineWidth);
				}
			}
			break;

			case FormType::Elipse:
			{
				if(f->getStates().Contains("RadiusX") && f->getStates().Contains("RadiusY"))
				{
					State radius[] = {f->getStates().Get("RadiusX"), f->getStates().Get("RadiusY")};
					float lineWidth = f->getStates().Contains("LineWidth") ? f->getStates().Get("LineWidth").as<float>() : 1.0f;
					gfx.DrawElipse(f->getPosition(), radius[0].as<float>(), radius[1].as<float>(),&f->getColor(), lineWidth);
				}
			}
			break;
			}
		}
	}
	

	InstanceID Form::_idCounter = 0;

	Form::Form()
		: _id(++_idCounter), _type(FormType::Null), _scale(1,1), _rotation(0), _position(0,0), 
		_color(255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true)
	{
	}

	Form::Form(const FormType& type)
		: _id(++_idCounter), _type(type), _scale(1,1), _rotation(0), _position(0,0), 
		_color(255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true)
	{
	}

	Form::Form(const Form& rhs)
		: _id(++_idCounter), _type(rhs._type), _scale(rhs._scale), _rotation(rhs._rotation), 
		_position(rhs._position), _color(rhs._color), _pivotPoint(rhs._pivotPoint), 
		_scalingCenter(rhs._scalingCenter), _visible(rhs._visible), _states(rhs._states)
	{
	}

	Form& Form::operator=(const Form& rhs)
	{
		if(this != &rhs)
		{
			_type = rhs._type;
			_states = rhs._states;
			_color = rhs._color;
			_position = rhs._position;
			_scalingCenter = rhs._scalingCenter;
			_scale = rhs._scale;
			_pivotPoint = rhs._pivotPoint;
			_rotation = rhs._rotation;
			_visible = rhs._visible;
		}
		return *this;
	}

	
	/********** GETTERS *****************/
	InstanceID Form::getID() const
	{
		return _id;
	}

	FormType Form::getType() const
	{
		return _type;
	}
	
	Statemap& Form::getStates()
	{
		return _states;
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

	void Form::setColor(uint r, uint g, uint b, uint a)
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
}