#ifndef WIN32_WINDOWCLASS_H_
#define WIN32_WINDOWCLASS_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
#include <windows.h>
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

#endif //WIN32_WINDOWCLASS_H_