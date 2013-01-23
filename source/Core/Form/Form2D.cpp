//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/Form/Form2D.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	InstanceID Form2D::_idCounter = 0;

	Form2D::Form2D()
		: _id(++_idCounter), _scale(1,1), _rotation(0), _position(0,0), _color(255,255,255,255), _pivotPoint(0,0), _scalingCenter(0,0), _visible(true)
	{
	}

	Form2D::~Form2D()
	{
	}

	Core::Statemap& Form2D::getStates()
	{
		return _states;
	}

	void Form2D::setColor(const Util::Color& color)
	{
		_color = color;
	}

	void Form2D::setPosition(const Util::Vec2& position)
	{
		_position = position;
	}

	void Form2D::setScalingCenter(const Util::Vec2& scaleCenter)
	{
		_scalingCenter = scaleCenter;
	}

	void Form2D::setScale(const Util::Vec2& scale)
	{
		_scale = scale;
	}

	void Form2D::setPivotPoint(const Util::Vec2& pivot)
	{
		_pivotPoint = pivot;
	}

	void Form2D::setRotation(const float rotation)
	{
		int sign = (rotation < 0) ? (-1) : (1);
		_rotation = std::abs(rotation) >= 360 ? rotation-sign*360 : rotation;
	}

	void Form2D::setVisibility(bool state)
	{
		_visible = state;
	}

	InstanceID Form2D::getID() const
	{
		return _id;
	}

	const Util::Color& Form2D::getColor() const
	{
		return _color;
	}

	const Util::Vec2& Form2D::getPosition() const
	{
		return _position;
	}

	const Util::Vec2& Form2D::getScalingCenter() const
	{
		return _scalingCenter;
	}

	const Util::Vec2& Form2D::getScale() const
	{
		return _scale;
	}

	const Util::Vec2& Form2D::getPivotPoint() const
	{
		return _pivotPoint;
	}

	float Form2D::getRotation() const
	{
		return _rotation;
	}

	bool Form2D::getVisibility() const
	{
		return _visible;
	}


	/*
	InstanceID Form::_idCounter = 0;

	InstanceID Form::_GenerateID()
	{
		return ++_idCounter;
	}

	Form::Form(Entity& owner)
		: _id(_GenerateID()), _parent(owner), _color(), _position(), _scale(Vector(1,1,1)), _rotation(0), _animated(false), _contents(Space::Make(owner, 1, 1, 1, 1)), _renderPriority(1)
	{
		getServices().getGraphics().AddToRenderQueue(this);
	}

	Form::~Form()
	{
		getServices().getGraphics().RemoveFromRenderQueue(this);
	}

	Form& Form::DrawSelf()
	{
		if(!_visible)
			return *this;

		//set some default Form data, and then let the concrete implementation draw itself
		IGraphics& ig = getServices().getGraphics();
		ig.SetColor(_color);
		ig.SetRotation(_rotation);
		ig.SetScale(_scale);
		ig.SetPosition(_position);
		
		OnDrawSelf();

		return *this;
	}

	Form& Form::AnimateSelf()
	{
		//can i do anything here? o.0
		if(_animated)
			OnAnimateSelf();
		return *this;
	}

	void Form::OnAnimateSelf()
	{}

	Form& Form::setAnimated(bool a)
	{
		_animated = a;
		return *this;
	}

	Form& Form::setColor(const Color& c)
	{
		_color = c;
		_color.Validate();
		return *this;
	}

	Form& Form::setContents(Space* s)
	{
		assert(s);
		_contents.reset(s);
		return *this;
	}

	Form& Form::setContents(spSpace s)
	{
		assert(s.get());
		_contents = s;
		return *this;
	}

	Form& Form::setPosition(const Vector& p)
	{
		_position = p;
		return *this;
	}

	Form& Form::setRotation(float r)
	{
		_rotation = r;
		return *this;
	}

	Form& Form::setScale(const Vector& s)
	{
		_scale = s;
		return *this;
	}

	Form& Form::setVisible(bool visible)
	{
		_visible = visible;
		return *this;
	}

	Form& Form::setRenderPriority(int p)
	{
		getServices().getGraphics().RemoveFromRenderQueue(this);
		_renderPriority = p;
		getServices().getGraphics().AddToRenderQueue(this);
		return *this;
	}

	//---------------------------------------------------------------------------
	InstanceID Form::getID() const
	{
		return _id;
	}

	Entity& Form::getParent() const
	{
		return _parent;
	}

	Statemap& Form::getStates()
	{
		return _states;
	}

	Space& Form::getContents()
	{
		return *_contents;
	}

	spSpace& Form::AcquireContents()
	{
		return _contents;
	}

	Color Form::getColor() const
	{
		return _color;
	}

	Vector Form::getPosition() const
	{
		return _position;
	}

	Vector Form::getScale() const
	{
		return _scale;
	}

	float Form::getRotation() const
	{
		return _rotation;
	}

	bool Form::isAnimated() const
	{
		return _animated;
	}

	bool Form::isVisible() const
	{
		return _visible;
	}

	int Form::getRenderPriority() const
	{
		return _renderPriority;
	}



	Form& Form::Translate(const Vector& t)
	{
		setPosition(_position + t);
		return *this;
	}

	Form& Form::Rotate(float rot)
	{
		setRotation(_rotation + rot);
		return *this;
	}

	Form& Form::Scale(const Vector& sc)
	{
		setScale(_scale + sc);
		return *this;
	}

	Form& Form::Colorize(const Color& c)
	{
		Color col = _color;
		col.red += c.red;
		col.green += c.green;
		col.blue += c.blue;
		col.alpha += c.alpha;
		setColor(col);
		return *this;
	}
	*/
}