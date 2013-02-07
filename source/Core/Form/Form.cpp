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
					State bounds = f->getStates()["Area"];
					gfx.DrawFont(font.as<int>(), text.as<String>(), &bounds.as<Util::Rect>());
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
	
	void Form::setPosition(const Util::Vec2& position)
	{
		_position = position;
	}

	void Form::setScalingCenter(const Util::Vec2& scaleCenter)
	{
		_scalingCenter = scaleCenter;
	}

	void Form::setScale(const Util::Vec2& scale)
	{
		_scale = scale;
	}

	void Form::setPivotPoint(const Util::Vec2& pivot)
	{
		_pivotPoint = pivot;
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
	
	void Form::Rotate(float rotation)
	{
		_rotation += rotation;
	}
	
	void Form::Translate(const Util::Vec2& translation)
	{
		_position += translation;
	}
}