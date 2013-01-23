#ifndef CORE_FORM_FORM2D_H_
#define CORE_FORM_FORM2D_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/State/Statemap.h"
#include "Util/Color.h"
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Graphics
{
	class Interface;
}

namespace Core
{
	class Form2D
	{
	private:
		static InstanceID _idCounter;
		
	protected:
		const InstanceID _id;
		Core::Statemap _states;
		Util::Color _color;
		Util::Vec2 _position;
		Util::Vec2 _scalingCenter;
		Util::Vec2 _scale;
		Util::Vec2 _pivotPoint;
		float _rotation;
		bool _visible;
	public:
		Form2D();
		virtual ~Form2D();

		virtual void DrawSelf(Graphics::Interface& gfx) = 0;
		virtual void AnimateSelf() = 0;
		virtual const String& getType() const = 0;
		
		Core::Statemap& getStates();

		void setColor(const Util::Color& color);
		void setPosition(const Util::Vec2& position);
		void setScalingCenter(const Util::Vec2& scaleCenter);
		void setScale(const Util::Vec2& scale);
		void setPivotPoint(const Util::Vec2& pivot);
		void setRotation(const float rotation);
		void setVisibility(bool state);

		const Util::Color& getColor() const;
		const Util::Vec2& getPosition() const;
		const Util::Vec2& getScalingCenter() const;
		const Util::Vec2& getScale() const;
		const Util::Vec2& getPivotPoint() const;
		float getRotation() const;
		bool getVisibility() const;
		InstanceID getID() const;
	};






	/*
	//fwds
	class Entity;
	//end fwds

	class Form : public ServiceHolder
	{
	private:
		static InstanceID _idCounter;
		static InstanceID _GenerateID();

		Form(Form& rhs);
		Form& operator=(Form& rhs);

		const InstanceID _id;
		Entity& _parent;
		
		Statemap _states;
		spSpace _contents;

		Color _color;
		Vector _position;
		Vector _scale;
		float _rotation;
		bool _animated;
		bool _visible;
		int _renderPriority;
		
		//perhaps i could save the matrices that DX makes anyway when it draws stuff
		//then i wouldn't need to make them every time i draw, + they only change when a value changes
		//but that's really an optimisation issue
		
		//3d stuff, (roll, yaw, pitch):float/matrix, (forward, right, up):vector3

	protected:
		virtual void OnDrawSelf() = 0;
		virtual void OnAnimateSelf();

		Form(Entity& owner);
		
	public:
		virtual ~Form();

		Form& DrawSelf();
		Form& AnimateSelf();

		virtual const String& getType() const = 0;
		InstanceID	getID() const;
		Entity&		getParent() const;
		Statemap&	getStates();
		Space&		getContents();
		Color		getColor() const;
		Vector		getPosition() const;
		Vector		getScale() const;
		float		getRotation() const;
		bool		isAnimated() const;
		bool		isVisible() const;
		int			getRenderPriority() const;

		spSpace& AcquireContents();

		Form& setAnimated(bool a);
		Form& setColor(const Color& c);
		Form& setContents(Space* s);
		Form& setContents(spSpace s);
		Form& setPosition(const Vector& p);
		Form& setRotation(float r);
		Form& setScale(const Vector& s);
		Form& setVisible(bool visible);
		Form& setRenderPriority(int p);

		Form& Translate(const Vector& t);
		Form& Rotate(float rot);
		Form& Scale(const Vector& sc);
		Form& Colorize(const Color& c);
		
	};
	*/
}

#endif //CORE_FORM_FORM2D_H_