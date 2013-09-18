#pragma once
/********************************************
	class:	$safeitemrootname$
	usage:	
********************************************/
	/*** common header ***/
#include "Engine/Defines.h"
#include "Engine/FwdDecl.h"
	/*** extra headers if needed ***/
	/*** end header inclusion ***/

namespace Input { class Event; }

class object_concept_t {
public:
	virtual ~object_concept_t() {};
	virtual bool handle(uint32_t,WPARAM,LPARAM,Input::Event&) = 0;
	virtual object_concept_t* copy() const = 0;
};
 
template <class T>
class model_t : public object_concept_t {
	T t;
public:
	model_t(T const &t) : t(t) {}
	model_t(T &&t) : t(std::move(t)) {}
	virtual bool handle(uint32_t m,WPARAM w,LPARAM l,Input::Event& e) { return t.handle(m,w,l,e); }
	virtual model_t* copy() const { return new model_t(*this); }
};
 
class object_t {
	std::unique_ptr<object_concept_t> object;
	friend bool handle(object_t const& o,uint32_t m,WPARAM w,LPARAM l,Input::Event& e) { return o.object->handle(m,w,l,e); }
public:
	object_t() {};
	object_t(object_t const &other) : object(other.object->copy()) {}
	object_t(object_t &&other) : object(std::move(other.object)) {}
  
	template <class O>
	object_t(O &&o) : object(new model_t<O>(o)) {}
  
	object_t& operator= (object_t const &other) { object_t copy(other); copy.swap(*this); return *this; }
	object_t& operator= (object_t &&other) { other.swap(*this); return *this; }

	bool handle(uint32_t m,WPARAM w,LPARAM l,Input::Event& e) { return object->handle(m,w,l,e); }
  
	void swap(object_t &other) {
		using std::swap;
		swap(other.object, this->object);
	}
};