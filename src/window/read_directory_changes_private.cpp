//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/read_directory_changes_private.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/utility.h"
#include "window/read_directory_changes.h"
/******* end headers *******/

namespace core
{
   // The namespace is a convenience to emphasize that these are internals
   // interfaces. The namespace can be safely removed if you need to.
   namespace RDCPrivate
   {
      CReadChangesRequest::CReadChangesRequest(CReadChangesServer* pServer,
                                               const char* dirName,
                                               BOOL trackSubdirs,
                                               DWORD trackFlags)
                                               : m_pServer(pServer), m_trackSubdirs(trackSubdirs),
                                               m_trackFlags(trackFlags), m_dirHandle(0)
      {
         strcpy(m_dirName, dirName);
         ::ZeroMemory(&m_overlapped, sizeof(OVERLAPPED));

         // The hEvent member is not used when there is a completion
         // function, so it's ok to use it to point to the object.
         m_overlapped.hEvent = this;
      }

      CReadChangesRequest::~CReadChangesRequest()
      {
         // RequestTermination() must have been called successfully.
         _ASSERTE(m_dirHandle == NULL);
      }

      bool CReadChangesRequest::OpenDirectory()
      {
         // Allow this routine to be called redundantly.
         if( m_dirHandle )
            return true;

         m_dirHandle = ::CreateFile(
            m_dirName,                                               // pointer to the file name
            FILE_LIST_DIRECTORY,                                     // access (read/write) mode
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,  // share mode
            NULL,                                                    // security descriptor
            OPEN_EXISTING,                                           // how to create
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,       // file attributes
            NULL);                                                   // file with attributes to copy

         if( m_dirHandle == INVALID_HANDLE_VALUE )
         {
            return false;
         }

         return true;
      }

      void CReadChangesRequest::BeginRead()
      {
         DWORD dwBytes = 0;

         // This call needs to be reissued after every APC.
         BOOL success = ::ReadDirectoryChangesW(
            m_dirHandle,               // handle to directory
            m_mainBuffer,              // read results buffer
            BufferSize,                // length of buffer
            m_trackSubdirs,            // monitoring option
            m_trackFlags,              // filter conditions
            &dwBytes,                  // bytes returned
            &m_overlapped,             // overlapped buffer
            &NotificationCompletion);  // completion routine
      }

      void CReadChangesRequest::BackupBuffer(DWORD dwSize)
      {
         // We could just swap back and forth between the two
         // buffers, but this code is easier to understand and debug.
         memset(m_backupBuffer, 0, BufferSize);
         memcpy(m_backupBuffer, m_mainBuffer, dwSize);
         memset(m_mainBuffer, 0, BufferSize);
      }

      VOID CALLBACK CReadChangesRequest::NotificationCompletion(
         DWORD dwErrorCode,               // completion code
         DWORD dwNumberOfBytesTransfered, // number of bytes transferred
         LPOVERLAPPED lpOverlapped)       // I/O information buffer
      {
         CReadChangesRequest* pBlock = (CReadChangesRequest*)lpOverlapped->hEvent;

         if( dwErrorCode == ERROR_OPERATION_ABORTED )
         {
            ::InterlockedDecrement(&pBlock->m_pServer->m_nOutstandingRequests);
            delete pBlock;
            return;
         }

         // Can't use sizeof(FILE_NOTIFY_INFORMATION) because
         // the structure is padded to 16 bytes.
         _ASSERTE(dwNumberOfBytesTransfered >= offsetof(FILE_NOTIFY_INFORMATION, FileName) + sizeof(WCHAR));

         // This might mean overflow? Not sure.
         if( !dwNumberOfBytesTransfered )
            return;

         pBlock->BackupBuffer(dwNumberOfBytesTransfered);

         // Get the new read issued as fast as possible. The documentation
         // says that the original OVERLAPPED structure will not be used
         // again once the completion routine is called.
         pBlock->BeginRead();

         pBlock->ProcessNotification();
      }

      void CReadChangesRequest::ProcessNotification()
      {
         char* pBase = (char*)m_backupBuffer;

         for( ;; )
         {
            FILE_NOTIFY_INFORMATION& fni = (FILE_NOTIFY_INFORMATION&)*pBase;
            char fileName[MAX_PATH] = {0};
            u32 written = sprintf(fileName, "%s\\", m_dirName);
            WideCharToMultiByte(CP_UTF8, 0, fni.FileName, fni.FileNameLength, fileName + written, MAX_PATH - written, 0, 0);

            auto attribs = GetFileAttributes(fileName);
            auto invalid = attribs == INVALID_FILE_ATTRIBUTES;
            auto isDir = (attribs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;

            if( !invalid && !isDir )
            {
               m_pServer->m_pParent->Push(fni.Action, fileName);
            }

            if( !fni.NextEntryOffset )
               break;
            pBase += fni.NextEntryOffset;
         };
      }

      void CReadChangesRequest::RequestTermination()
      {
         ::CancelIo(m_dirHandle);
         ::CloseHandle(m_dirHandle);
         m_dirHandle = nullptr;
      }





      CReadChangesServer::CReadChangesServer(CReadDirectoryChanges* pParent)
         : m_bTerminate(false), m_nOutstandingRequests(0), m_pParent(pParent), m_freeSlot(0)
      {}

      u32 WINAPI CReadChangesServer::ThreadStartProc(LPVOID arg)
      {
         CReadChangesServer* pServer = (CReadChangesServer*)arg;
         pServer->Run();
         return 0;
      }

      void CALLBACK CReadChangesServer::TerminateProc(__in ULONG_PTR arg)
      {
         CReadChangesServer* pServer = (CReadChangesServer*)arg;
         pServer->RequestTermination();
      }

      void CALLBACK CReadChangesServer::AddDirectoryProc(__in ULONG_PTR arg)
      {
         CReadChangesRequest* pRequest = (CReadChangesRequest*)arg;
         pRequest->m_pServer->AddDirectory(pRequest);
      }

      void CReadChangesServer::Run()
      {
         while( m_nOutstandingRequests || !m_bTerminate )
         {
            DWORD rc = ::SleepEx(INFINITE, true);
         }
      }

      void CReadChangesServer::AddDirectory(CReadChangesRequest* pBlock)
      {
         CORE_ASSERT_DBGERR(m_freeSlot < MaxBlocks, "Reached maximum number of directory watch blocks");
         if( pBlock->OpenDirectory() )
         {
            ::InterlockedIncrement(&pBlock->m_pServer->m_nOutstandingRequests);
            m_pBlocks[m_freeSlot++] = pBlock;
            pBlock->BeginRead();
         }
         else
            delete pBlock;
      }

      void CReadChangesServer::RequestTermination()
      {
         m_bTerminate = true;

         for( DWORD i = 0; i < m_freeSlot; ++i )
         {
            // Each Request object will delete itself.
            m_pBlocks[i]->RequestTermination();
         }

         m_freeSlot = 0;
      }
   }
}