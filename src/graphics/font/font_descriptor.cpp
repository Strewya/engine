//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/font/font_descriptor.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "lua/lua_system.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   FontDescriptor loadFont(LuaStack lua, const char* filename, HTexture texture, HVertexShader vshader, HPixelShader pshader)
   {
      FontDescriptor result{};

      if( !lua.doFile(filename) )
      {
         auto str = lua.to<std::string>();
         lua.pop();
         CORE_INFO(str);
         return result;
      }

      for( lua.pairs(); lua.next(); lua.pop(1) )
      {
         if( !lua.is<std::string>(-2) || !lua.is<LuaTable>(-1) )
         {
            return result;
         }
         result.height = get(lua, "size", 0);
         if( result.height == 0 )
         {
            return result;
         }
         for( lua.ipairs("glyphs"); lua.next(); lua.pop(1) )
         {
            if( lua.is<uint32_t>(-2) && lua.is<LuaTable>(-1) )
            {
               auto ascii = get<char>(lua, "char", 0);
               auto left = get(lua, "left", -1);
               auto right = get(lua, "right", -1);
               auto top = get(lua, "top", -1);
               if( ascii != 0 && left != -1 && right != -1 && top != -1 )
               {
                  auto i = ascii - 32;
                  result.glyphs[i].center = {(left + right)*0.5f, top + result.height*0.5f};
                  result.glyphs[i].halfSize = {(right - left)*0.5f, result.height*0.5f};
               }
            }
         }
         result.fontTexture = texture;
         result.vshader = vshader;
         result.pshader = pshader;
      }
      lua.pop();
      assert(lua.getTop() == 0);

      return result;
   }
}