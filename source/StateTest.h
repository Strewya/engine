#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <algorithm>
#include <memory>
#include <typeinfo>
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/Dimensional.h"
	/*** end header inclusion ***/

namespace Core
{
namespace StateTest
{
	struct BaseState
	{
		const type_info& type;
		
		BaseState(const type_info& type) : type(type) {}

		template<typename T> T* downcast()
		{
			return static_cast<T*>(this);
		}
	};

	template<typename DATA, typename CRTP> struct State : public BaseState
	{
		typedef DATA Data_t;
		typedef CRTP Derived_t;
		static size_t hash;

		Data_t value;

		State() : BaseState(typeid(Derived_t)) { if(hash == 0) hash = type.hash_code(); }
	};


	struct Position : public State<Util::Vec2, Position>
	{};

	template<typename T, typename U> size_t State<T,U>::hash = 0;
}
}