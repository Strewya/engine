//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Scripting/LuaSystem.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Scripting/luaBinding.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
/******* end headers *******/

namespace Core
{
	int luaPrint(lua_State* L)
	{
		int32_t n = lua_gettop(L);
		for( int32_t i = 1; i <= n; ++i )
		{
			switch( lua_type(L, i) )
			{
				case LUA_TNUMBER:
					std::cout << lua_tonumber(L, i);
					break;
				case LUA_TSTRING:
					std::cout << lua_tostring(L, i);
					break;
				case LUA_TBOOLEAN:
					std::cout << (lua_toboolean(L, i) == 1 ? "true" : "false");
					break;
				default:
					std::cout << lua_typename(L, i) << "(" << lua_tonumber(L, i) << ")";
					break;
			}
		}
		std::cout << std::endl;
		return 0;
	}

	int luaErrHandler(lua_State* L)
	{
		lua_pushnil(L);
		return 2;
	}

	bool LuaSystem::init()
	{
		DEBUG_STATUS(false);
		m_L = luaL_newstate();
		if( m_L )
		{
			luaL_openlibs(m_L);
			tolua_core_open(m_L);

			DEBUG_CODE_START
				lua_register(m_L, "print", luaPrint);
			DEBUG_CODE_END;

			const char* depend = R"rawLuaCode(
Lua = {};
global_dependency_table = {};
global_dependency_missing = nil;

function depend(filename)
	if(not global_dependency_table[filename]) then
		global_dependency_missing = filename;
		error('Missing dependency');
	end;
end;

function class(name)
	Lua[name] = {};
	Lua[name].__index = Lua[name];
	setmetatable(Lua[name], { __call = function(cls, ...) return cls.new(...); end; });
	Lua[name].new = function()
		return {};
	end;
end
)rawLuaCode";

			status = luaL_dostring(m_L, depend) == 0;
		}

		DEBUG_INIT(LuaSystem);
	}

	bool LuaSystem::shutdown()
	{
		bool status = true;
		lua_close(m_L);
		DEBUG_SHUTDOWN(LuaSystem);
		return status;
	}

	void LuaSystem::update()
	{
		lua_gc(m_L, LUA_GCSTEP, 20);
	}

	LuaStack LuaSystem::getStack() const
	{
		return LuaStack(m_L);
	}
}
