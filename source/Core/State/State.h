#ifndef CORE_STATE_STATE_H_
#define CORE_STATE_STATE_H_
#define STATE_VERSION 2
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <algorithm>
#include <memory>
#include <typeinfo>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Core
{
	template<typename T> class State;

	class IState
	{
	protected:
		const std::type_info& _type;
	public:
		IState(const std::type_info& typeInfo) : _type(typeInfo) {};
		virtual ~IState() {};

		template<typename T> T& as();
		template<typename T> static std::unique_ptr<State<T>> Create(const T& value);
	};

	/*
		State class, version 3
		Description:
			A much more simple version of the State functionality.
			States must be created by the new keyword.
			A potential method might be to make the constructor private and make a static method that creates the state instance.
			This change would allow optimizations in the construction of the state instance, as it could use an object or memory pool for the instances.			
	*/
	template<typename T> class State : public IState
	{
	public:
		T value;
		
		State()
			: IState(typeid(T))
		{}

		State(const T& value)
			: IState(typeid(T)), value(value)
		{}

		State(const State& rhs)
			: IState(rhs._type)
		{}

		State& operator=(const State& rhs)
		{
			if(this != &rhs)
			{
				value = rhs.value;
			}
			return *this;
		}

		~State() {};
	};

	template<typename T> T& IState::as()
	{
		if(typeid(T) == _type)
		{
			return static_cast<State<T>*>(this)->value;
		}
		throw std::exception("State::as<T>(): Tried to access wrong type!");
	}

	template<typename T> static std::unique_ptr<State<T>> IState::Create(const T& value)
	{
		return (std::unique_ptr<State<T>>(new State<T>(value)));
	}
}

#endif //CORE_STATE_STATE_H_