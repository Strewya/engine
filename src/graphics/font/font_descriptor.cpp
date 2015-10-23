//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/font/font_descriptor.h"
/******* c++ headers *******/
/******* extra headers *******/
#include "lua/lua_system.h"
#include "graphics/graphics_system.h"
#include "utility/utility.h"
/******* end headers *******/

namespace core
{
   HFont loadFont(LuaStack lua, GraphicsSystem* gfx, char* definitionFile)
   {
      HFont result{};

      if( !lua.doFile(definitionFile) )
      {
         auto str = lua.to<str_writeable>();
         lua.pop();
         CORE_LOG(str);
         return result;
      }

      FontDescriptor fd{};
      for( lua.pairs(); lua.next(); lua.pop(1) )
      {
         if( !lua.is<str>(-2) || !lua.is<LuaTable>(-1) )
         {
            return result;
         }
         fd.height = get(lua, "size", 0);
         if( fd.height == 0 )
         {
            return result;
         }
         for( lua.ipairs("glyphs"); lua.next(); lua.pop(1) )
         {
            if( lua.is<u32>(-2) && lua.is<LuaTable>(-1) )
            {
               auto ascii = get<char>(lua, "char", 0);
               auto left = get(lua, "left", -1);
               auto right = get(lua, "right", -1);
               auto top = get(lua, "top", -1);
               if( ascii != 0 && left != -1 && right != -1 && top != -1 )
               {
                  auto i = ascii - 32;
                  fd.glyphs[i].center = {(left + right)*0.5f, top + fd.height*0.5f};
                  fd.glyphs[i].halfSize = {(right - left)*0.5f, fd.height*0.5f};
               }
            }
         }
         /*
         fd.fontTexture = texture;
         fd.vshader = vshader;
         fd.pshader = pshader;
         */
      }
      lua.pop();
      CORE_ASSERT_DBGERR(lua.getTop() == 0, "Font loading did not clean up the Lua stack correctly");

      return result;
   }
}