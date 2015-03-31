//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Graphics/DXRenderer.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
   bool DXRenderer::init()
   {
      CORE_STATUS(true);
      CORE_INIT(DXRenderer);
   }

   bool DXRenderer::shutdown()
   {
      CORE_STATUS(true);
      CORE_SHUTDOWN(DXRenderer);
   }
}
