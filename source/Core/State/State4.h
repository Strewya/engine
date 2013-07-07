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
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Core
{
//namespace StateTest
//{
	class BaseState
	{
	public:
		const size_t uid;
	
		BaseState(const size_t uid);
		virtual ~BaseState();

		virtual std::unique_ptr<BaseState> clone() const = 0;
		template<typename T> const T* cast() const;
		template<typename T> bool cast(const T** outState) const;
	};




	template<typename DATA, typename CRTP> struct State : public BaseState
	{
		typedef DATA Data_t;
		typedef CRTP Derived_t;
		static size_t Type;

		Data_t value;

		State() : BaseState(Type) {}
		State(const Data_t& val) : BaseState(Type), value(val) {}
		std::unique_ptr<BaseState> clone() const;
	};





	template<typename DATA, typename CRTP> size_t State<DATA,CRTP>::Type = typeid(CRTP).hash_code();


	template<typename T> const T* BaseState::cast() const
	{
		return static_cast<const T*>(this);
	}
	
	template<typename T> bool BaseState::cast(const T** outState) const
	{
		if(outState != nullptr && T::Type == uid)
		{
			*outState = static_cast<const T*>(this);
			return true;
		}
		return false;
	}

	template<typename DATA, typename CRTP> std::unique_ptr<BaseState> State<DATA,CRTP>::clone() const
	{
		return std::unique_ptr<CRTP>(new CRTP(value));
	}



	template<typename T, typename D> std::unique_ptr<T> create(const D& data)
	{
		return std::unique_ptr<T>(new T(data));
	}



}
//}