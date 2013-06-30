#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
#include <string>
#include <windows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Win32
{
	class AbstractWindow
	{
	public:
		static LRESULT CALLBACK MessageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		AbstractWindow();
		virtual ~AbstractWindow() {};

		virtual LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)=0;
		virtual bool Create();
		virtual void Show();
		virtual bool Update();
		void Shutdown();

		void setFullscreen(bool fs);
		void setExtendedStyle(uint32_t style);
		void setClass(const String& name);
		void setClass(const char* name);
		void setTitle(const String& name);
		void setTitle(const char* name);
		void setStyle(uint32_t style);
		void setPosition(int x, int y);
		void setSize(int x, int y);
		void setParent(HWND parent);
		void setMenu(HMENU menu);
		void setInstance(HINSTANCE instance);
		void setUsePeekMessage(bool peek);
		void setUseWaitMessage(bool wait);
		void setEventQueue(std::list<Input::Event>& queue);

		HWND getWindowHandle() const;
		bool getFullscreen() const;
		uint32_t getExtendedStyle() const;
		const String& getClass() const;
		const String& getTitle() const;
		uint32_t getStyle() const;
		int getPositionX() const;
		int getPositionY() const;
		int getSizeX() const;
		int getSizeY() const;
		HWND getParent() const;
		HMENU getMenu() const;
		HINSTANCE getInstance() const;
		bool getUsePeekMessage() const;
		bool getUseWaitMessage() const;
		int getExitCode() const;

	protected:
		HWND _hwnd;

		bool _usePeekMessage;
		bool _useWaitMessage;
		bool _fullscreen;
		int _exitCode;

		uint32_t _extendedStyle;
		String _class;
		String _title;
		uint32_t _style;
		int _xPos;
		int _yPos;
		int _xSize;
		int _ySize;
		HWND _hwndParent;
		HMENU _hMenu;
		HINSTANCE _hInstance;

		std::list<Input::Event>* _queue;
	};

	int InitializeWindow(AbstractWindow& window, bool fullscreen, uint32_t width, uint32_t height);
}