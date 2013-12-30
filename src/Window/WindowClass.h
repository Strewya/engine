#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Window/myWindows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <string>
	/*** end header inclusion ***/

namespace Core
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

		ATOM registerClass();
		const char* getClassName() const;
	};

}