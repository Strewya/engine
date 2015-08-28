#pragma once
/********************************************
*  contents:   cache for binding names to handles
*  usage:
********************************************/
/******* c++ headers *******/
#include <string>
#include <unordered_map>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace core
{
   template<typename HANDLE>
   struct NameCache
   {
   public:
      HANDLE getHandle(const char* name) const
      {
         auto it = m_handles.find(name);
         if( it != m_handles.end() )
         {
            return it->second;
         }
         return HANDLE{};
      }

      void bind(const char* name, HANDLE handle)
      {
         m_handles.emplace(name, handle);
      }

      void unbind(const char* name)
      {
         m_handles.erase(name);
      }

      void unbind(HANDLE handle)
      {
         for( auto it = m_handles.begin(); it != m_handles.end(); ++it )
         {
            if( it->second == handle )
            {
               m_handles.erase(it);
               break;
            }
         }
      }

   private:
      typedef std::unordered_map<std::string, HANDLE> HandleMap;

      HandleMap m_handles;
   };
}
