#pragma once
/********************************************
*  contents:   resource manager template class
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
/******* common headers *******/
#include "utility/types.h"
/******* extra headers *******/
#include "utility/template/resource_cache_template.h"
#include "utility/template/name_cache_template.h"
#include "utility/utility.h"
/******* end header inclusion *******/

namespace core
{
   template<typename DATA, typename HANDLE, typename LOADER>
   struct Manager
   {
   public:
      bool init(const char* type, LOADER& loader, DATA defaultData);
      bool shutdown();

      template<typename ...Args>
      HANDLE loadFromFile(const char* filename, Args&&... rest);
      HANDLE loadFromFile(const char* filename);

      template<typename ...Args>
      void reloadFromFile(const char* filename, Args&&... rest);
      void reloadFromFile(const char* filename);

      bool isLoaded(const char* filename) const;

      DATA& getData(HANDLE handle);
      const DATA& getData(HANDLE handle) const;

      void release(HANDLE handle);

   private:
      typedef Cache<DATA, HANDLE> Cache_t;
      typedef NameCache<HANDLE> Names_t;

      Cache_t m_data;
      Names_t m_names;
      DATA m_defaultData;

      LOADER* m_loader;
      const char* m_type;
   };

   template<typename DATA, typename HANDLE, typename LOADER>
   bool Manager<DATA, HANDLE, LOADER>::init(const char* type, LOADER& loader, DATA defaultData)
   {
      CORE_INIT_START(ManagerTemplate);

      m_type = type;
      m_defaultData = defaultData;
      m_loader = &loader;

      CORE_STATUS_AND(m_loader != nullptr);
      CORE_STATUS_AND(m_defaultData.loaded());

      CORE_INIT_END;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   bool Manager<DATA, HANDLE, LOADER>::shutdown()
   {
      CORE_SHUTDOWN_START(ManagerTemplate);

      //unload all existing data
      m_loader->unload(m_defaultData);

      //to not have memory leaks on shutdown, but still check if all resources were cleaned up in the game code
      for( auto& data : m_data )
      {
         m_loader->unload(data);
      }

      CORE_STATUS_AND(m_defaultData.unloaded());
      CORE_STATUS_AND(m_data.hasUsedHandles() == false);

      CORE_SHUTDOWN_END;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   template<typename ...Args>
   HANDLE Manager<DATA, HANDLE, LOADER>::loadFromFile(const char* filename, Args&&... rest)
   {
      HANDLE handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_loader->load(filename, std::forward<Args>(rest)...);
         if( loadedData.loaded() )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   HANDLE Manager<DATA, HANDLE, LOADER>::loadFromFile(const char* filename)
   {
      HANDLE handle = m_names.getHandle(filename);
      if( handle.isNull() )
      {
         auto loadedData = m_loader->load(filename);
         if( loadedData.loaded() )
         {
            auto& data = m_data.acquire(handle);
            data = loadedData;
            m_names.bind(filename, handle);
         }
      }
      return handle;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   template<typename ...Args>
   void Manager<DATA, HANDLE, LOADER>::reloadFromFile(const char* filename, Args&&... rest)
   {
      auto handle = m_names.getHandle(filename);
      if( !handle.isNull() )
      {
         auto* data = m_data.dereference(handle);
         if( data )
         {
            auto loadedData = m_loader->load(filename, std::forward<Args>(rest)...);
            if( loadedData.loaded() )
            {
               m_loader->unload(*data);
               *data = loadedData;
            }
         }
      }
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   void Manager<DATA, HANDLE, LOADER>::reloadFromFile(const char* filename)
   {
      auto handle = m_names.getHandle(filename);
      if( !handle.isNull() )
      {
         auto* data = m_data.dereference(handle);
         if( data )
         {
            auto loadedData = m_loader->load(filename);
            if( loadedData.loaded() )
            {
               m_loader->unload(*data);
               *data = loadedData;
            }
         }
      }
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   bool Manager<DATA, HANDLE, LOADER>::isLoaded(const char* filename) const
   {
      auto handle = m_names.getHandle(filename);
      return handle.isNull() == false;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   DATA& Manager<DATA, HANDLE, LOADER>::getData(HANDLE handle)
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultData;
      }
      return *data;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   const DATA& Manager<DATA, HANDLE, LOADER>::getData(HANDLE handle) const
   {
      auto* data = m_data.dereference(handle);
      if( data == nullptr )
      {
         return m_defaultData;
      }
      return *data;
   }

   template<typename DATA, typename HANDLE, typename LOADER>
   void Manager<DATA, HANDLE, LOADER>::release(HANDLE handle)
   {
      auto* data = m_data.dereference(handle);
      if( data != nullptr )
      {
         m_loader->unload(*data);
         m_names.unbind(handle);
         m_data.release(handle);
      }
   }
}
