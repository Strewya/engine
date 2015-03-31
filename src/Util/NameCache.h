#pragma once
/********************************************
*  contents:   NameCache
*  usage:
********************************************/
/******* C++ headers *******/
#include <string>
#include <unordered_map>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
   template<typename HANDLE>
   class NameCache
   {
   public:
      HANDLE getHandle(const std::string& name) const
      {
         auto it = m_handles.find(name);
         if( it != m_handles.end() )
         {
            return it->second;
         }
         return HANDLE{};
      }

      void bind(const std::string& name, HANDLE handle)
      {
         m_handles.emplace(name, handle);
      }

      void unbind(const std::string& name)
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
