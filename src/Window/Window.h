#pragma once
/********************************************
*	class:	Window
*	usage:
********************************************/
/******* C++ headers *******/
#include <deque>
#include <list>
#include <string>
#include <Window/myWindows.h>
/******* common headers *******/
/******* extra headers *******/
#include <Util/Time.h>
#include <Window/WindowEvent.h>
#include <Window/ReadDirectoryChanges.h>
/******* end header inclusion *******/

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
		~Window();
		
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
		WindowEvent newEvent();

		HWND m_hwnd;

		HANDLE m_dirHandle;
		DWORD m_trackedChanges;
		
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
		bool m_updateWindow;

		std::string m_class;
		std::string m_title;
		std::string m_resourcesDirectory;

		Time m_timer;
		CReadDirectoryChanges m_monitor;
		std::list<WindowEvent> m_events;

	public:
		void addStringForPaint(const std::string& str)
		{
			if(m_drawStrings.size() > 10)
			{
				m_drawStrings.pop_back();
			}
			m_drawStrings.emplace_front(str);
			m_updateWindow = true;
		}

		bool getNextChangedFile(std::string& outStr)
		{
			if(!m_fileChanges.empty())
			{
				outStr = m_fileChanges.front();
				m_fileChanges.pop_front();
				return true;
			}
			return false;
		}

		void processFileChanges();

	private:
		std::unordered_map<std::string, uint64_t> m_fileChangeBuffer;
		std::list<std::string> m_fileChanges;
		std::deque<std::string> m_drawStrings;
	};

	
}
