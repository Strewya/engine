//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Window/Window.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Util/TimeUnitConversions.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   WindowEvent& Window::newEvent()
   {
      m_clock.update();
      WindowEvent& we = m_events[m_headIndex];
      ZeroMemory(&we, sizeof(WindowEvent));
      we.m_timestamp = m_clock.getCurrentMicros();
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

   void Window::newFileChange(uint64_t timestamp, DWORD action, const std::string& file)
   {
      using std::begin;
      using std::end;

      auto it = std::find_if(begin(m_fileChanges), end(m_fileChanges), [&](const FileChangeInfo& info)
      {
         return info.m_state == FileChangeInfo::EVENT_PENDING && info.m_filename == file && info.m_action == action;
      });

      if( it != end(m_fileChanges) )
      {
         it->m_timestamp = timestamp;
         return;
      }

      auto& info = m_fileChanges[m_nextFreeSlot];
      info.m_timestamp = timestamp;
      info.m_action = action;
      info.m_filename.assign(file);
      info.m_state = FileChangeInfo::EVENT_PENDING;
      m_nextFreeSlot = (m_nextFreeSlot + 1) % m_fileChanges.size();
      if( m_fileChanges[m_nextFreeSlot].m_state == FileChangeInfo::EVENT_PENDING )
      {
         CORE_INFO("WHOOPS, overwriting a previous file change. This is a BAD THING! Maybe we "
                   "should increase the size of our buffer from ", m_fileChanges.size(), "...");
      }
   }

   void Window::processFileChanges()
   {
      std::string file;
      DWORD action;
      while( m_monitor.Pop(action, file) )
      {
         if( !file.empty() && file.find(".") != file.npos )
         {
            m_clock.update();
            newFileChange(m_clock.getCurrentMicros(), action, file);
         }
      }

      using std::begin;
      using std::end;
      m_clock.update();
      std::for_each(begin(m_fileChanges), end(m_fileChanges), [&](FileChangeInfo& info)
      {
         if( info.m_state == FileChangeInfo::EVENT_PENDING &&
            m_clock.getCurrentMicros() > info.m_timestamp + milisToMicros(m_fileChangeDelay) )
         {
            info.m_state = FileChangeInfo::READ_PENDING;
            auto& we = newEvent();
            we.m_type = WindowEventType::WE_FILECHANGE;
            we.m_fileChange.m_index = info.m_index;
            writeEvent();
         }
      });
   }

   void Window::calculateClientRect(uint32_t& outXSize, uint32_t& outYSize)
   {
      RECT rc = {0, 0, m_xSize, m_ySize};
      AdjustWindowRectEx(&rc, m_style, false, m_extendedStyle);
      outXSize = rc.right - rc.left;
      outYSize = rc.bottom - rc.top;
   }
}
