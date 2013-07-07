#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <functional>
#include <memory>
#include <typeinfo>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
namespace State_v3
{
	template<typename T> class ValueState;
	
	typedef std::function<void(InstanceID)> Deleter;
	void deleter_noop(InstanceID handle);


	class State
	{
	public:
		State(const std::type_info& typeInfo);
		virtual ~State();

		virtual std::unique_ptr<State> Clone() = 0;
		template<typename T> T& as();
		template<typename T> bool isType();
		template<typename T> static std::unique_ptr<ValueState<T>> Create(const T& value);
	
	protected:
		const std::type_info& _type;
	};

	/*
		State class, version 3
		Description:
			A much more simple version of the State functionality.
			States must be created by the new keyword.
			A potential method might be to make the constructor private and make a static method that creates the state instance.
			This change would allow optimizations in the construction of the state instance, as it could use an object or memory pool for the instances.			
	*/
	template<typename T> class ValueState : public State
	{
	public:
		T value;
		
		ValueState(const T& value);
		ValueState(const ValueState& rhs);
		ValueState& operator=(const ValueState& rhs);
		~ValueState();

		std::unique_ptr<State> Clone();
	};


	//****************************************************************************************
	//			STATE IMPLEMENTATION
	//****************************************************************************************
	template<typename T> T& State::as()
	{
		if(typeid(T) == _type)
		{
			return static_cast<ValueState<T>*>(this)->value;
		}
		throw std::exception("State::as<T>(): Tried to access wrong type!");
	}

	template<typename T> bool State::isType()
	{
		return typeid(T) == _type;
	}

	template<typename T> std::unique_ptr<ValueState<T>> State::Create(const T& value)
	{
		return (std::unique_ptr<ValueState<T>>(new ValueState<T>(value)));
	}

	//****************************************************************************************
	//			VALUE STATE IMPLEMENTATION
	//****************************************************************************************
	template<typename T> ValueState<T>::ValueState(const T& value)
		: State(typeid(T)), value(value)
	{}

	template<typename T> ValueState<T>::ValueState(const ValueState& rhs)
		: State(rhs._type), value(rhs.value)
	{}

	template<typename T> ValueState<T>& ValueState<T>::operator=(const ValueState& rhs)
	{
		if(this != &rhs)
		{
			value = rhs.value;
		}
		return *this;
	}

	template<typename T> ValueState<T>::~ValueState() {};

	template<typename T> std::unique_ptr<State> ValueState<T>::Clone()
	{
		return std::unique_ptr<ValueState<T>>(new ValueState<T>(*this));
	}
}
}