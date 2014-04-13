#pragma once
/********************************************
*	class:	Window
*	usage:
********************************************/
/******* C++ headers *******/
#include <list>
#include <string>
#include <vector>
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
		bool getChangedFile(uint32_t index, uint32_t& outAction, std::string& outStr);

		void processFileChanges();
		
		void showCursor(bool isShown);
		void setFullscreen(bool isFullscreen);

		void setExtendedStyle(uint32_t style);
		void setStyle(uint32_t style);
		void setFileChangeDelay(uint32_t delay);
		
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
		
		uint32_t getFileChangeDelay() const;

		bool peekEvent(uint64_t time, WindowEvent& outEvent);
#ifdef _DEBUG
		void openConsole(uint32_t xPos, uint32_t yPos);
		void closeConsole();
#endif
	protected:	
		WindowEvent newEvent();
		void newFileChange(uint64_t timestamp, DWORD action, const std::string& file);

		DWORD m_trackedChanges;
		int32_t m_xPos;
		int32_t m_yPos;
		int32_t m_xSize;
		int32_t m_ySize;
		uint32_t m_exitCode;
		uint32_t m_style;
		uint32_t m_extendedStyle;
		uint32_t m_minFileChangeDelay;
		uint32_t m_fileChangeDelay;

		HWND m_hwnd;

		bool m_fullscreen;
		bool m_showCursor;
		bool m_isRunning;
		
		std::string m_class;
		std::string m_title;
		std::string m_resourcesDirectory;

		std::list<WindowEvent> m_events;

		struct FileChangeInfo
		{
			enum State
			{
				UNUSED,
				EVENT_PENDING,
				READ_PENDING
			};
			FileChangeInfo(uint32_t index)
			: m_timestamp(0), m_action(0), m_index(index), m_filename(), m_state(UNUSED)
			{}

			uint64_t		m_timestamp;
			DWORD			m_action;
			const uint32_t	m_index;
			State			m_state;
			std::string		m_filename;
		};

		std::vector<FileChangeInfo> m_fileChanges;
		uint32_t m_nextFreeSlot;

		Time m_timer;
		CReadDirectoryChanges m_monitor;
	};

	
}
