#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
#include <memory>
#include <typeinfo>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	class State
	{
	public:
		const InstanceID uid;
	
		State(InstanceID uid);
		virtual ~State();

		virtual std::unique_ptr<State> clone() const = 0;
		template<typename T> const T* cast() const;
		template<typename T> T* cast();
		template<typename T> bool cast(const T** outState) const;
		template<typename T> bool cast(T** outState);
	};




	template<typename T> struct StateType : public State
	{
		typedef T Derived_t;
		typedef std::unique_ptr<T> Uptr;
		typedef T* Rptr;
		typedef T& Ref;

		static const InstanceID Type;

		static std::unique_ptr<Derived_t> create()
		{
			std::unique_ptr<Derived_t> ptr(new Derived_t());
			return ptr;
		}

		std::unique_ptr<State> clone() const 
		{
			return typeClone();
		}

		std::unique_ptr<Derived_t> typeClone() const
		{
			return std::unique_ptr<Derived_t>(new Derived_t(*this->cast<Derived_t>()));
		}

	protected:
		StateType() : State(Type) {}
	};





	template<typename T> const InstanceID StateType<T>::Type = typeid(T).hash_code();


	template<typename T> const T* State::cast() const
	{
		if(T::Type != this->uid)
		{
			return nullptr;
		}
		return static_cast<const T*>(this);
	}
	
	template<typename T> bool State::cast(const T** outState) const
	{
		if(outState != nullptr)
		{
			*outState = this->cast<T>();
			return true;
		}
		return false;
	}

	template<typename T> T* State::cast()
	{
		if(T::Type != this->uid)
		{
			return nullptr;
		}
		return static_cast<T*>(this);
	}
	
	template<typename T> bool State::cast(T** outState)
	{
		if(outState != nullptr)
		{
			*outState = this->cast<T>();
			return true;
		}
		return false;
	}
}

#define COMPONENT(Name) struct Name : public Core::StateType<Name>