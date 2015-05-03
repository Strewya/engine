#pragma once
/********************************************
*  contents:   code that tracks file changes for a specified directory
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
#include <vector>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   class CReadDirectoryChanges;

   namespace RDCPrivate
   {
      class CReadChangesServer;

      ///////////////////////////////////////////////////////////////////////////

      // All functions in CReadChangesRequest run in the context of the worker thread.
      // One instance of this object is created for each call to AddDirectory().
      class CReadChangesRequest
      {
      public:
         CReadChangesRequest(CReadChangesServer* pServer, const std::string& dirName, BOOL trackSubdirs, DWORD trackFlags, DWORD bufferSize);

         ~CReadChangesRequest();

         bool OpenDirectory();

         void BeginRead();

         // The dwSize is the actual number of bytes sent to the APC.
         void BackupBuffer(DWORD bytesToCopy);

         void ProcessNotification();

         void RequestTermination();

         CReadChangesServer* m_pServer;

      protected:

         static VOID CALLBACK NotificationCompletion(
            DWORD dwErrorCode,               // completion code
            DWORD dwNumberOfBytesTransfered, // number of bytes transferred
            LPOVERLAPPED lpOverlapped);      // I/O information buffer

         // Parameters from the caller for ReadDirectoryChangesW().
         DWORD       m_trackFlags;
         DWORD       m_bufferSize;
         BOOL        m_trackSubdirs;
         std::string m_dirName;

         // Result of calling CreateFile().
         HANDLE      m_dirHandle;

         // Required parameter for ReadDirectoryChangesW().
         OVERLAPPED  m_overlapped;

         // Data buffer for the request.
         // Since the memory is allocated by malloc, it will always
         // be aligned as required by ReadDirectoryChangesW().
         std::vector<BYTE> m_mainBuffer;

         // Double buffer strategy so that we can issue a new read
         // request before we process the current buffer.
         std::vector<BYTE> m_backupBuffer;
      };

      ///////////////////////////////////////////////////////////////////////////

      // All functions in CReadChangesServer run in the context of the worker thread.
      // One instance of this object is allocated for each instance of CReadDirectoryChanges.
      // This class is responsible for thread startup, orderly thread shutdown, and shimming
      // the various C++ member functions with C-style Win32 functions.
      class CReadChangesServer
      {
      public:
         CReadChangesServer(CReadDirectoryChanges* parent);

         static unsigned int WINAPI ThreadStartProc(LPVOID arg);

         // Called by QueueUserAPC to start orderly shutdown.
         static void CALLBACK TerminateProc(__in  ULONG_PTR arg);

         // Called by QueueUserAPC to add another directory.
         static void CALLBACK AddDirectoryProc(__in  ULONG_PTR arg);

         CReadDirectoryChanges* m_pParent;

         volatile DWORD m_nOutstandingRequests;

      protected:

         void Run();

         void AddDirectory(CReadChangesRequest* pBlock);

         void RequestTermination();

         std::vector<CReadChangesRequest*> m_pBlocks;

         bool m_bTerminate;
      };

   }
}