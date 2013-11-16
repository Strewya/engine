#pragma once
/********************************************
	class:	DeviceContext
	usage:	
********************************************/
	/*** common header ***/
#include <Engine/Defines.h>
#include <Engine/FwdDecl.h>
	/*** extra headers if needed ***/
#include <deque>
#include <wtypes.h>
	/*** end header inclusion ***/

#define HANDLE_PARAMS HWND h, uint32_t m, WPARAM w, LPARAM l, std::deque<Input::Event>& q

namespace Input
{
	class Event;

	class device_concept_t
	{
	public:
		virtual ~device_concept_t() {};
		virtual device_concept_t* copy() const = 0;
		//specific functionality
		virtual bool handle(HANDLE_PARAMS) = 0;
	};
 
	template <class T> class device_model_t : public device_concept_t
	{
		T t;
	public:
		device_model_t(T const &t) : t(t) {}
		device_model_t(T &&t) : t(std::move(t)) {}
		virtual device_model_t* copy() const { return new device_model_t(*this); }
		//specific functionality
		virtual bool handle(HANDLE_PARAMS) { return t.handle(h,m,w,l,q); }
	};
 
	class device_t {
		std::unique_ptr<device_concept_t> object;
		friend bool handle(device_t const& o,HANDLE_PARAMS) { return o.object->handle(h,m,w,l,q); }
	public:
		device_t() {};
		device_t(device_t const &other) : object(other.object->copy()) {}
		device_t(device_t &&other) : object(std::move(other.object)) {}
  
		template <class O> device_t(O &&o) : object(new device_model_t<O>(o)) {}
  
		device_t& operator= (device_t other) { other.swap(*this); return *this; }
		device_t& operator= (device_t &&other) { other.swap(*this); return *this; }

		void swap(device_t &other) {
			std::swap(other.object, this->object);
		}

		//specifc functionality
		bool handle(HANDLE_PARAMS) { return object->handle(h,m,w,l,q); }
  
		
	};
}

#undef HANDLE_PARAMS