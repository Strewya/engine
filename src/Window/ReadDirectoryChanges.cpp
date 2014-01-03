//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Window/ReadDirectoryChanges.h>
/******* C++ headers *******/
#include <process.h>
/******* extra headers *******/
#include <Window/ReadDirectoryChangesPrivate.h>
/******* end headers *******/

namespace Core
{
	///////////////////////////////////////////////////////////////////////////
	// CReadDirectoryChanges

	CReadDirectoryChanges::CReadDirectoryChanges()
		: m_notifications(), m_threadHandle(nullptr), m_threadId(0),
		m_pServer(std::make_unique<RDCPrivate::CReadChangesServer>(this))
	{
	}

	CReadDirectoryChanges::~CReadDirectoryChanges()
	{
		Terminate();
	}

	void CReadDirectoryChanges::Init()
	{
		//
		// Kick off the worker thread, which will be
		// managed by CReadChangesServer.
		//
		m_threadHandle = (HANDLE)_beginthreadex(nullptr,
			0,
			RDCPrivate::CReadChangesServer::ThreadStartProc,
			m_pServer.get(),
			0,
			&m_threadId
			);
	}

	void CReadDirectoryChanges::Terminate()
	{
		if(m_threadHandle)
		{
			::QueueUserAPC(RDCPrivate::CReadChangesServer::TerminateProc, m_threadHandle, (ULONG_PTR)m_pServer.get());
			::WaitForSingleObjectEx(m_threadHandle, 10000, true);
			::CloseHandle(m_threadHandle);

			m_threadHandle = nullptr;
			m_threadId = 0;
		}
	}

	void CReadDirectoryChanges::AddDirectory(const std::string& watchedDir, BOOL watchSubdirs, DWORD trackFlags, DWORD bufferSize)
	{
		if(!m_threadHandle)
			Init();

		auto* pRequest = new RDCPrivate::CReadChangesRequest(m_pServer.get(), watchedDir, watchSubdirs, trackFlags, bufferSize);
		QueueUserAPC(RDCPrivate::CReadChangesServer::AddDirectoryProc, m_threadHandle, (ULONG_PTR)pRequest);
	}

	void CReadDirectoryChanges::Push(DWORD action, const std::string& filename)
	{
		m_notifications.push_back(TDirectoryChangeNotification(action, filename));
	}

	bool CReadDirectoryChanges::Pop(DWORD& outAction, std::string& outFilename)
	{
		bool found = false;
		if(!m_notifications.empty())
		{
			found = true;
			outAction = m_notifications.front().first;
			outFilename = m_notifications.front().second;
			m_notifications.pop_front();
		}
		return found;
	}

	bool CReadDirectoryChanges::CheckOverflow()
	{
		bool b = false;
		if(b)
			m_notifications.clear();
		return b;
	}

	uint32_t CReadDirectoryChanges::GetThreadId()
	{
		return m_threadId;
	}
}