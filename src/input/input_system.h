#pragma once
/********************************************
*  contents:   input system
*  usage:
********************************************/
/******* c++ headers *******/
#include <vector>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   struct Clock;
   struct WinMsg;
   struct CommunicationBuffer;

   typedef std::vector<WinMsg> EventVector_t;

   struct InputSystem
   {
   public:
      bool init();
      bool shutdown();

      void gatherInputForCurrentFrame(u64 pollMicros, CommunicationBuffer* comm);

      const EventVector_t& getEvents() const;

   private:
      EventVector_t m_inputEvents;
   };
}
