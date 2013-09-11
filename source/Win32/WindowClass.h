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
		WindowClass(const std::string& className);
		WindowClass(HINSTANCE hInst, const std::string& className);

		ATOM Register();
		const char* getClassName() const;
	};

}