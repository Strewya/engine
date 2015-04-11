//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Window/Window.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Util/Clock.h>
#include <Util/TimeUnitConversions.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   FileChangeType toFileChangeType(DWORD action);

   WindowEvent& Window::newEvent()
   {
      WindowEvent& we = m_events[m_headIndex];
      ZeroMemory(&we, sizeof(WindowEvent));
      we.timestamp = Clock::getRealTimeMicros();
      return we;
   }

   void Window::writeEvent()
   {
      if( m_headIndex == m_tailIndex )
      {
         CORE_INFO("WHOOPS, overwriting a previous event. This is a BAD THING! Maybe we "
                   "should increase the size of our buffer from ", m_eventQueueSize, "...");
      }

      m_headIndex = (m_headIndex + 1) % m_eventQueueSize;
   }

   void Window::processFileChanges()
   {
      std::string file;
      DWORD action;
      while( m_monitor.Pop(action, file) )
      {
         if( file.size() < FilenameStringSize && file.find(".") != file.npos )
         {
            auto& we = newEvent();
            we.type = WE_FILECHANGE;
            we.fileChange.action = toFileChangeType(action);
            strncpy(we.fileChange.filename, file.c_str(), FilenameStringSize);
            we.fileChange.filename[FilenameStringSize] = 0;
            writeEvent();
         }
         else
         {
            CORE_INFO("Filename too long (", file.size(), ") or not a file(", file, ")");
         }
      }
   }

   void Window::calculateClientRect(uint32_t& outXSize, uint32_t& outYSize)
   {
      RECT rc = {0, 0, m_xSize, m_ySize};
      AdjustWindowRectEx(&rc, m_style, false, m_extendedStyle);
      outXSize = rc.right - rc.left;
      outYSize = rc.bottom - rc.top;
   }

   FileChangeType toFileChangeType(DWORD action)
   {
      FileChangeType returnValue = Core::FILE_BADDATA;
      switch( action )
      {
         case FILE_ACTION_ADDED:
         {
            returnValue = Core::FILE_ADDED;
         }
         break;

         case FILE_ACTION_MODIFIED:
         {
            returnValue = Core::FILE_MODIFIED;
         }
         break;

         case FILE_ACTION_REMOVED:
         {
            returnValue = Core::FILE_REMOVED;
         }
         break;

         case FILE_ACTION_RENAMED_OLD_NAME:
         {
            returnValue = Core::FILE_RENAMED_FROM;
         }
         break;

         case FILE_ACTION_RENAMED_NEW_NAME:
         {
            returnValue = Core::FILE_RENAMED_TO;
         }
         break;

         default:
            break;
      }
      return returnValue;
   }
}
