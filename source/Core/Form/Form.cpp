//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Form/Form.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	enum class FormType
	{
		Font,
		Sprite,
		Texture,
		Model,
		Shader
	};
	
	
	void DrawForms(std::vector<Form*>& frms, Graphics::Interface& gfx)
	{
		for(Form* f : frms)
		{
			switch(f->getType())
			{
			case FormType::Font:
			{
				State text = f->getStates().Get("Text");
				State font = f->getStates().Get("Font");
				State bounds = f->getStates().Get("Area");
				gfx.DrawFont(font.as<int>(), text.as<String>(), &bounds.as<Rect>());
			}
			break;
			
			case FormType::Sprite:
			{
				State sheet = f->getStates().Get("Spritesheet");
				State currentFrame = f->getStates().Get("CurrentFrame");
				gfx->setTransform2D(f->getPosition(), f->getScaleCenter(), ...);
				gfx.DrawSprite(sheet.getTextureHandle(), sheet.getSprite(currentFrame));
			}
			break;
			
			case FormType::Texture:
			{
			}
			break;
			
			default:
				
			}
		}
	}
	
	
	

	InstanceID Form::_idCounter = 0;

	Form::Form(const FormType& type)
		: _id(++_idCounter), _type(type), _scale(1,1), _rotation(0), _position(0,0), _color(255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true)
	{
	}

	
	/********** GETTERS *****************/
	InstanceID Form::getID() const
	{
		return _id;
	}

	const FormType& Form::getType() const
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