#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <list>
#include <string>
#include "Win32/myWindows.h"
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Input { class Event; }

namespace Win32
{
	class Window
	{
	public:
		static LRESULT CALLBACK MessageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		Window();
		Window(const char* title);
		Window(const std::string& title);
		virtual ~Window();

		LRESULT CALLBACK WindowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
		virtual bool Create();
		virtual void Show();
		virtual bool Update();
		void Shutdown();
		void showMessagebox(const char* title, const char* text);

		void setFullscreen(bool fs);
		void setExtendedStyle(uint32_t style);
		void setClass(const std::string& name);
		void setClass(const char* name);
		void setTitle(const std::string& name);
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
		void setCursorShow(bool isShown);

		HWND getWindowHandle() const;
		bool getFullscreen() const;
		uint32_t getExtendedStyle() const;
		const std::string& getClass() const;
		const std::string& getTitle() const;
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
		bool isCursorShown() const;

	protected:
		HWND _hwnd;
		HWND _hwndParent;
		HMENU _hMenu;
		HINSTANCE _hInstance;

		bool _usePeekMessage;
		bool _useWaitMessage;
		bool _fullscreen;
		bool _showCursor;

		uint32_t _exitCode;
		uint32_t _style;
		uint32_t _extendedStyle;

		int32_t _xPos;
		int32_t _yPos;
		int32_t _xSize;
		int32_t _ySize;

		std::string _class;
		std::string _title;

		std::list<Input::Event>* _queue;
	};

	int InitializeWindow(Window& window, bool fullscreen, uint32_t width, uint32_t height);
}
