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
         std::remove_if(m_handles.begin(), m_handles.end(), [=](const HandleMap::value_type& value)
         {
            return value.second == handle;
         });
      }

   private:
      typedef std::unordered_map<std::string, HANDLE> HandleMap;

      HandleMap m_handles;
   };
}
