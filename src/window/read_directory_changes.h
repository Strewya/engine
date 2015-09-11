#pragma once
/********************************************
*  contents:   code that tracks file changes for a specified directory
*  usage:
********************************************/
/******* c++ headers *******/
#include <atomic>
#include <memory>
#include <string>
#include <vector>
/******* common headers *******/
#include "utility/types.h"
#include "window/window_include.h"
/******* extra headers *******/
#include "window/read_directory_changes_private.h"
/******* end header inclusion *******/

namespace core
{
   namespace RDCPrivate
   {
      struct CReadChangesServer;
   }

   struct CReadDirectoryChanges
   {
   public:
      enum
      {
         MaxNotifications = 128,
      };
      struct TDirectoryChangeNotification
      {
         DWORD first;
         char second[MAX_PATH];
      };

      CReadDirectoryChanges();
      ~CReadDirectoryChanges();

      void Init();
      void Terminate();

      void AddDirectory(const char* watchedDir, BOOL watchSubdirs, DWORD trackFlags);

      bool Pop(DWORD& outAction, char* buffer, u32 bufferLength);

      void Push(DWORD action, const char* filename);

      bool CheckOverflow();

      u32 GetThreadId();

   protected:
      RDCPrivate::CReadChangesServer m_pServer;

      HANDLE m_threadHandle;

      u32 m_threadId;

      TDirectoryChangeNotification m_notifications[MaxNotifications];
      std::atomic<u32> m_head;
      std::atomic<u32> m_tail;
   };
}