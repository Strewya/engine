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
      HANDLE getHandle(std::string name)
      {
         auto it = m_handles.find(name);
         if( it != m_handles.end() )
         {
            return it->second;
         }
         return HANDLE{};
      }

      void bind(std::string name, HANDLE handle)
      {
         m_handles.emplace(name, handle);
      }

      void unbind(std::string name)
      {
         m_handles.erase(name);
      }

   private:
      typedef std::unordered_map<std::string, HANDLE> HandleMap;

      HandleMap m_handles;
   };
}
