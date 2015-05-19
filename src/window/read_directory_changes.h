#pragma once
/********************************************
*  contents:   code that tracks file changes for a specified directory
*  usage:
********************************************/
/******* c++ headers *******/
#include <memory>
#include <string>
#include <vector>
/******* common headers *******/
#include "window/window_include.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
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

      std::vector<TDirectoryChangeNotification> m_notifications;
      uint32_t m_notificationQueueSize;
      uint32_t m_head;
      uint32_t m_tail;
   };
}