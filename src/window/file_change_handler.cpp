//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "window/file_change_handler.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "utility/communication_buffer.h"
/******* end headers *******/

namespace core
{
   core_internal FileChangeType toFileChangeType(DWORD action)
   {
      FileChangeType returnValue = core::FileChangeType::BadData;
      switch( action )
      {
         case FILE_ACTION_ADDED:
         {
            returnValue = core::FileChangeType::Added;
         }
         break;
         case FILE_ACTION_MODIFIED:
         {
            returnValue = core::FileChangeType::Modified;
         }
         break;
         case FILE_ACTION_REMOVED:
         {
            returnValue = core::FileChangeType::Removed;
         }
         break;
         case FILE_ACTION_RENAMED_OLD_NAME:
         {
            returnValue = core::FileChangeType::RenamedFrom;
         }
         break;
         case FILE_ACTION_RENAMED_NEW_NAME:
         {
            returnValue = core::FileChangeType::RenamedTo;
         }
         break;
         default:
         {
         } break;
      }
      return returnValue;
   }

   void FileChangeHandler::handle(CommunicationBuffer* buffer)
   {
      WinMsg we{};
      DWORD action;
      while( m_monitor.Pop(action, we.fileChange.name, FileChangeEvent::NameStringSize) )
      {
         we.type = WinMsgType::FileChange;
         we.fileChange.action = toFileChangeType(action);
         we.fileChange.name[FileChangeEvent::NameStringSize] = 0;
         buffer->writeEvent(we);
      }
   }

   void FileChangeHandler::addDirectory(const char* fullPathDirectory, u32 trackedChanges)
   {
      m_monitor.AddDirectory(fullPathDirectory, true, trackedChanges);
   }
}
