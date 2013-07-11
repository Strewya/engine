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
				
			}
			break;
			
			case FormType::Sprite:
			{
				
			}
			break;
			
			case FormType::Texture:
			{
				
			}
			break;

			case FormType::Line:
			{
				
			}
			break;

			case FormType::Triangle:
			{
				
			}
			break;

			case FormType::Rectangle:
			{
				
			}
			break;

			case FormType::Circle:
			{
				
			}
			break;

			case FormType::Elipse:
			{
				
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

	bool Form::Insert(StateUptr state)
	{
		return _states.insert(std::move(state));
	}

	void Form::ClearStates()
	{
		_states.clear();
	}

	bool Form::RemoveState(InstanceID id)
	{
		return _states.destroy(id);
	}

	BaseState* Form::getState(InstanceID id)
	{
		return _states.retrieve(id);
	}

	bool Form::hasState(InstanceID id, bool recursive)
	{
		return _states.contains(id);
	}
}