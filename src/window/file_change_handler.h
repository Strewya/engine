#pragma once
/********************************************
*  contents:   handler for file change events
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "window/window_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "window/read_directory_changes.h"
#include "window/window_message.h"
/******* end header inclusion *******/

namespace core
{
   struct CommunicationBuffer;

   struct FileChangeHandler
   {
   public:

      void addDirectory(const char* fullPathDirectory, u32 trackedChanges);
      void handle(CommunicationBuffer* buffer);

   private:
      // #memory still uses the standard allocation strategies (new, malloc, unique_ptr). convert at some point.
      CReadDirectoryChanges m_monitor;
   };
}
