#ifndef CORE_FORM_FORM_H_
#define CORE_FORM_FORM_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Core/Space/Space.h"
#include "Core/State/Statemap.h"
#include "Util/Color.h"
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Core
{
	class ResourceLocator;
	class ServiceLocator;
}

namespace Core
{
	enum class FormType
	{
		Null,
		Line,
		Triangle,
		Rectangle,
		Circle,
		Elipse,
		Font,
		Sprite,
		Texture
	};
	
	class Form
	{
	private:
		FormType _type;
		Util::Color _color;
		Util::Vec2 _position;
		Util::Vec2 _scalingCenter;
		Util::Vec2 _scale;
		Util::Vec2 _pivotPoint;
		float _rotation;
		bool _visible;
		
		Statemap _states;
		Space _space;
		//perhaps i could save the matrices that DX makes anyway when it draws stuff
		//then i wouldn't need to make them every time i draw, + they only change when a value changes
		//but that's really an optimisation issue
		
		//3d stuff, (roll, yaw, pitch):float/matrix, (forward, right, up):vector3
		//the Vec2 usage above for position, scale and such will be a problem for 3D data
		
		Form(const Form& rhs);
		Form(Form&& rhs);
		Form& operator=(Form&& rhs);
		Form& operator=(const Form& rhs);

	public:
		Form();
		Form(const FormType& type);
		
		
		Space&				getSpace();
		FormType			getType() const;
		float 				getRotation() const;
		bool 				getVisibility() const;
		const Util::Color& 	getColor() const;
		const Util::Vec2& 	getPosition() const;
		const Util::Vec2& 	getScalingCenter() const;
		const Util::Vec2& 	getScale() const;
		const Util::Vec2& 	getPivotPoint() const;
		
		void setType(const FormType& type);
		void setColor(const Util::Color& color);
		void setColor(uint r, uint g, uint b, uint a=255);
		void setPosition(const Util::Vec2& position);
		void setPosition(float x, float y);
		void setScalingCenter(const Util::Vec2& scaleCenter);
		void setScalingCenter(float x, float y);
		void setScale(const Util::Vec2& scale);
		void setScale(float x, float y);
		void setPivotPoint(const Util::Vec2& pivot);
		void setPivotPoint(float x, float y);
		void setRotation(float rotation);
		void setVisibility(bool state);
		
		void Scale(const Util::Vec2& scaling);
		void Scale(float x, float y);
		void Rotate(float rotation);
		void Translate(const Util::Vec2& translation);
		void Translate(float x, float y);

		

		void ClearStates();
		bool RemoveState(const char* name);
		bool RemoveState(const String& name);
		bool Insert(const char* name, std::unique_ptr<State> state);
		bool Insert(const String& name, std::unique_ptr<State> state);
		State* getState(const char* name);
		State* getState(const String& name);
		bool hasState(const char* name, bool recursive = false);
		bool hasState(const String& name, bool recursive = false);
	};

	void DrawForms(const std::deque<Form*>& frms, const Core::ServiceLocator& services, const Core::ResourceLocator& resources);
}

#endif //CORE_FORM_FORM_H_