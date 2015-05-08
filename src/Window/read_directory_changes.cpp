//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/read_directory_changes.h"
/******* c++ headers *******/
#include <process.h>
/******* extra headers *******/
#include "window/read_directory_changes_private.h"
/******* end headers *******/

namespace core
{
   ///////////////////////////////////////////////////////////////////////////
   // CReadDirectoryChanges

   CReadDirectoryChanges::CReadDirectoryChanges()
      : m_notifications(), m_threadHandle(nullptr), m_threadId(0),
      m_pServer(std::make_unique<RDCPrivate::CReadChangesServer>(this)),
      m_notificationQueueSize(128), m_head(1), m_tail(0)
   {
      m_notifications.resize(m_notificationQueueSize);
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
      if( m_threadHandle )
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
      if( !m_threadHandle )
         Init();

      auto* pRequest = new RDCPrivate::CReadChangesRequest(m_pServer.get(), watchedDir, watchSubdirs, trackFlags, bufferSize);
      QueueUserAPC(RDCPrivate::CReadChangesServer::AddDirectoryProc, m_threadHandle, (ULONG_PTR)pRequest);
   }

   void CReadDirectoryChanges::Push(DWORD action, const std::string& filename)
   {
      m_head = (m_head + 1) % m_notificationQueueSize;

      m_notifications[m_head].first = action;
      m_notifications[m_head].second = filename;
   }

   bool CReadDirectoryChanges::Pop(DWORD& outAction, std::string& outFilename)
   {
      bool eventExists = false;
      auto readIndex = (m_tail + 1) % m_notificationQueueSize;
      if( readIndex != m_head )
      {
         eventExists = true;
         outAction = m_notifications[readIndex].first;
         outFilename = m_notifications[readIndex].second;
         m_tail = readIndex;
      }
      return eventExists;
   }

   bool CReadDirectoryChanges::CheckOverflow()
   {
      return false;
   }

   uint32_t CReadDirectoryChanges::GetThreadId()
   {
      return m_threadId;
   }
}