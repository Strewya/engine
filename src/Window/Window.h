#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <list>
#include <string>
#include <Window/myWindows.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Util/Time.h>
#include <Window/WindowEvent.h>
	/*** end header inclusion ***/

namespace Core
{
	class Window;

	enum WindowResult
	{
		OK = 0,
		WindowClassRegistrationError,
		WindowCreationError,
		WindowNotExistsError,
		WindowClosing
	};

	int initializeWindow(Window& window);

	class Window
	{
	public:
		static LRESULT CALLBACK messageRouter(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

		Window();
		Window(const char* title);
		Window(const std::string& title);
		
		LRESULT CALLBACK windowProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
		bool create();
		void show();
		void update();
		void close();
		
		void showMessagebox(const char* title, const char* text);
		void resize(uint32_t x, uint32_t y);
		void move(int32_t x, int32_t y);
		void calculateClientRect(uint32_t& outXSize, uint32_t& outYSize);
		
		void showCursor(bool isShown);
		void setFullscreen(bool isFullscreen);

		void setExtendedStyle(uint32_t style);
		void setStyle(uint32_t style);
		
		HWND getWindowHandle() const;
		uint32_t getStyle() const;
		uint32_t getExtendedStyle() const;
		const std::string& getClass() const;
		const std::string& getTitle() const;
		int32_t getPositionX() const;
		int32_t getPositionY() const;
		uint32_t getSizeX() const;
		uint32_t getSizeY() const;
		int32_t getExitCode() const;
		bool isCursorShown() const;
		bool isFullscreen() const;
		bool isRunning() const;

		bool peek(uint64_t time, WindowEvent& outEvent);

	protected:
		HWND m_hwnd;
		
		uint32_t m_exitCode;
		uint32_t m_style;
		uint32_t m_extendedStyle;

		int32_t m_xPos;
		int32_t m_yPos;
		int32_t m_xSize;
		int32_t m_ySize;

		bool m_fullscreen;
		bool m_showCursor;
		bool m_isRunning;

		std::string m_class;
		std::string m_title;

		Time m_timer;
		std::list<WindowEvent> m_events;
	};

	
}
