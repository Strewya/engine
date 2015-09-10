//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/read_directory_changes.h"
/******* c++ headers *******/
#include <process.h>
/******* extra headers *******/
#include "utility/utility.h"
#include "window/read_directory_changes_private.h"
/******* end headers *******/

namespace core
{
   ///////////////////////////////////////////////////////////////////////////
   // CReadDirectoryChanges

   CReadDirectoryChanges::CReadDirectoryChanges()
      : m_threadHandle(nullptr), m_threadId(0),
      m_pServer(this),
      m_head(1), m_tail(0)
   {
      memset(m_notifications, 0, sizeof(m_notifications));
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
                                              &m_pServer,
                                              0,
                                              &m_threadId
                                              );
   }

   void CReadDirectoryChanges::Terminate()
   {
      if( m_threadHandle )
      {
         ::QueueUserAPC(RDCPrivate::CReadChangesServer::TerminateProc, m_threadHandle, (ULONG_PTR)&m_pServer);
         ::WaitForSingleObjectEx(m_threadHandle, 10000, true);
         ::CloseHandle(m_threadHandle);

         m_threadHandle = nullptr;
         m_threadId = 0;
      }
   }

   void CReadDirectoryChanges::AddDirectory(const char* watchedDir, BOOL watchSubdirs, DWORD trackFlags)
   {
      if( !m_threadHandle )
         Init();

      auto* pRequest = new RDCPrivate::CReadChangesRequest(&m_pServer, watchedDir, watchSubdirs, trackFlags);
      QueueUserAPC(RDCPrivate::CReadChangesServer::AddDirectoryProc, m_threadHandle, (ULONG_PTR)pRequest);
   }

   void CReadDirectoryChanges::Push(DWORD action, const char* filename)
   {
      auto writeIndex = m_head % MaxNotifications;

      m_notifications[writeIndex].first = action;
      strcpy(m_notifications[writeIndex].second, filename);
      ++m_head;
   }

   bool CReadDirectoryChanges::Pop(DWORD& outAction, char* buffer, u32 bufferLength)
   {
      bool eventExists = false;
      if( m_tail + 1 != m_head )
      {
         auto readIndex = (m_tail + 1) % MaxNotifications;
         CORE_ASSERT_FATAL_DEBUG(strlen(m_notifications[readIndex].second) < bufferLength, "Buffer too small for filename");
         outAction = m_notifications[readIndex].first;
         strcpy(buffer, m_notifications[readIndex].second);
         ++m_tail;
         eventExists = true;
      }
      return eventExists;
   }

   bool CReadDirectoryChanges::CheckOverflow()
   {
      return false;
   }

   u32 CReadDirectoryChanges::GetThreadId()
   {
      return m_threadId;
   }
}