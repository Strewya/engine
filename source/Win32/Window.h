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
		typedef std::function<bool(HWND,uint32_t,WPARAM,LPARAM)> Listener;
		static LRESULT CALLBACK messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		Window();
		Window(const char* title);
		Window(const std::string& title);
		virtual ~Window();

		LRESULT CALLBACK windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
		virtual bool create();
		virtual void show();
		virtual bool update();
		void shutdown();
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
		void setCursorShow(bool isShown);
		void registerListener(Listener listener);

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
		HWND m_hwnd;
		HWND m_hwndParent;
		HMENU m_hMenu;
		HINSTANCE m_hInstance;

		bool m_usePeekMessage;
		bool m_useWaitMessage;
		bool m_fullscreen;
		bool m_showCursor;

		uint32_t m_exitCode;
		uint32_t m_style;
		uint32_t m_extendedStyle;

		int32_t m_xPos;
		int32_t m_yPos;
		int32_t m_xSize;
		int32_t m_ySize;

		std::string m_class;
		std::string m_title;

		std::list<Listener> m_listeners;
	};

	int initializeWindow(Window& window, bool fullscreen, uint32_t width, uint32_t height);
}
