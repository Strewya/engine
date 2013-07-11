#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <memory>
#include <typeinfo>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
//namespace StateTest
//{
	class BaseState
	{
	public:
		const InstanceID uid;
	
		BaseState(InstanceID uid);
		virtual ~BaseState();

		virtual std::unique_ptr<BaseState> clone() const = 0;
		template<typename T> const T* cast() const;
		template<typename T> T* cast();
		template<typename T> bool cast(const T** outState) const;
		template<typename T> bool cast(T** outState);
	};




	template<typename T> struct State : public BaseState
	{
		typedef T Derived_t;
		typedef std::unique_ptr<T> Uptr;
		typedef T* Rptr;
		typedef T& Ref;

		static const InstanceID Type;

		static std::unique_ptr<Derived_t> create()
		{
			return std::unique_ptr<Derived_t>(new Derived_t());
		}

		std::unique_ptr<BaseState> clone() const 
		{
			return typeClone();
		}

		std::unique_ptr<Derived_t> typeClone() const
		{
			return std::unique_ptr<Derived_t>(new Derived_t(*this->cast<Derived_t>()));
		}

	protected:
		State() : BaseState(Type) {}
	};





	template<typename T> const InstanceID State<T>::Type = typeid(T).hash_code();


	template<typename T> const T* BaseState::cast() const
	{
		if(T::Type != this->uid)
		{
			return nullptr;
		}
		return static_cast<const T*>(this);
	}
	
	template<typename T> bool BaseState::cast(const T** outState) const
	{
		if(outState != nullptr)
		{
			*outState = this->cast<T>();
			return true;
		}
		return false;
	}

	template<typename T> T* BaseState::cast()
	{
		if(T::Type != this->uid)
		{
			return nullptr;
		}
		return static_cast<T*>(this);
	}
	
	template<typename T> bool BaseState::cast(T** outState)
	{
		if(outState != nullptr)
		{
			*outState = this->cast<T>();
			return true;
		}
		return false;
	}
}
//}