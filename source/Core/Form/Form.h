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
	enum class FormType
	{
		Null,
		Font,
		Sprite,
		Texture,
		Model,
		Shader
	};
	
	class Form
	{
	private:
		static InstanceID _idCounter;
		
		const InstanceID _id;
		FormType _type;
		Statemap _states;
		Util::Color _color;
		Util::Vec2 _position;
		Util::Vec2 _scalingCenter;
		Util::Vec2 _scale;
		Util::Vec2 _pivotPoint;
		float _rotation;
		bool _visible;
		
		//perhaps i could save the matrices that DX makes anyway when it draws stuff
		//then i wouldn't need to make them every time i draw, + they only change when a value changes
		//but that's really an optimisation issue
		
		//3d stuff, (roll, yaw, pitch):float/matrix, (forward, right, up):vector3
		//the Vec2 usage above for position, scale and such will be a problem for 3D data
		
	public:
		Form();
		Form(const FormType& type);
		Form(const Form& rhs);
		Form& operator=(const Form& rhs);
		

		InstanceID 	getID() const;
		FormType	getType() const;
		Statemap& 	getStates();
		float 		getRotation() const;
		bool 		getVisibility() const;
		const Util::Color& 	getColor() const;
		const Util::Vec2& 	getPosition() const;
		const Util::Vec2& 	getScalingCenter() const;
		const Util::Vec2& 	getScale() const;
		const Util::Vec2& 	getPivotPoint() const;
		
		
		void setType(const FormType& type);
		void setColor(const Util::Color& color);
		void setPosition(const Util::Vec2& position);
		void setScalingCenter(const Util::Vec2& scaleCenter);
		void setScale(const Util::Vec2& scale);
		void setPivotPoint(const Util::Vec2& pivot);
		void setRotation(float rotation);
		void setVisibility(bool state);
		
		void Scale(const Util::Vec2& scaling);
		void Rotate(float rotation);
		void Translate(const Util::Vec2& translation);
	};
	
	void DrawForms(const std::vector<Form*>& frms, Graphics::Interface& gfx);
}

#endif //CORE_FORM_FORM2D_H_