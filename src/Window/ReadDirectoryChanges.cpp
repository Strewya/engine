

#include <stdafx.h>
#include <process.h>
#include <Window/ReadDirectoryChanges.h>
#include <Window/ReadDirectoryChangesPrivate.h>

using namespace ReadDirectoryChangesPrivate;

///////////////////////////////////////////////////////////////////////////
// CReadDirectoryChanges

CReadDirectoryChanges::CReadDirectoryChanges(int nMaxCount)
	: m_Notifications(nMaxCount)
{
	m_hThread	= NULL;
	m_dwThreadId= 0;
	m_pServer	= new CReadChangesServer(this);
}

CReadDirectoryChanges::~CReadDirectoryChanges()
{
	Terminate();
	delete m_pServer;
}

void CReadDirectoryChanges::Init() 
{
	//
	// Kick off the worker thread, which will be
	// managed by CReadChangesServer.
	//
	m_hThread = (HANDLE)_beginthreadex(NULL,
		0,
		CReadChangesServer::ThreadStartProc,
		m_pServer,
		0,
		&m_dwThreadId
		);
}

void CReadDirectoryChanges::Terminate() 
{
	if (m_hThread)
	{
		::QueueUserAPC(CReadChangesServer::TerminateProc, m_hThread, (ULONG_PTR)m_pServer);
		::WaitForSingleObjectEx(m_hThread, 10000, true);
		::CloseHandle(m_hThread);

		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

void CReadDirectoryChanges::AddDirectory( const std::string& szDirectory, BOOL bWatchSubtree, DWORD dwNotifyFilter, DWORD dwBufferSize ) 
{
	if (!m_hThread)
		Init();

	CReadChangesRequest* pRequest = new CReadChangesRequest(m_pServer, szDirectory, bWatchSubtree, dwNotifyFilter, dwBufferSize);
	QueueUserAPC(CReadChangesServer::AddDirectoryProc, m_hThread, (ULONG_PTR)pRequest);
}

void CReadDirectoryChanges::Push(DWORD dwAction, const std::string& wstrFilename) 
{
	m_Notifications.push_back( TDirectoryChangeNotification(dwAction, wstrFilename) );
}

bool  CReadDirectoryChanges::Pop(DWORD& outAction, std::string& outFilename) 
{
	TDirectoryChangeNotification pair;
	if (m_Notifications.empty())
		return false;

	outAction = m_Notifications.front().first;
	outFilename = m_Notifications.front().second;
	m_Notifications.pop_front();

	return true;
}

bool CReadDirectoryChanges::CheckOverflow()
{
	bool b = false;
	if (b)
		m_Notifications.clear();
	return b;
}
