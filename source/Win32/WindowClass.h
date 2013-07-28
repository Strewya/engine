#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include "Win32/myWindows.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Win32
{
	class WindowClass : public WNDCLASSEX
	{
	private:
		void FillDefaultData();

	public:
		WindowClass(const char* className);
		WindowClass(HINSTANCE hInst, const char* className);
		WindowClass(const String& className);
		WindowClass(HINSTANCE hInst, const String& className);

		ATOM Register();
		const char* getClassName() const;
	};

}