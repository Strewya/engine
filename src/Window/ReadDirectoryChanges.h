#pragma once
/********************************************
*	class:	CReadDirectoryChanges
*	usage:
********************************************/
/******* C++ headers *******/
#include <list>
#include <memory>
#include <string>
#include <Window/myWindows.h>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	namespace RDCPrivate
	{
		class CReadChangesServer;
	}

	class CReadDirectoryChanges
	{
	public:
		typedef std::pair<DWORD, std::string> TDirectoryChangeNotification;

		CReadDirectoryChanges();
		~CReadDirectoryChanges();

		void Init();
		void Terminate();

		void AddDirectory(const std::string& watchedDir, BOOL watchSubdirs, DWORD trackFlags, DWORD bufferSize = 16384);

		bool Pop(DWORD& outAction, std::string& outFilename);

		void Push(DWORD action, const std::string& filename);

		bool CheckOverflow();

		uint32_t GetThreadId();

	protected:
		std::unique_ptr<RDCPrivate::CReadChangesServer> m_pServer;

		HANDLE m_threadHandle;

		uint32_t m_threadId;

		std::list<TDirectoryChangeNotification> m_notifications;
	};
}