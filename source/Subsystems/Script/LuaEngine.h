#ifndef SCRIPT_LUAENGINE_H_
#define SCRIPT_LUAENGINE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Subsystems/Script/LuaInclude.h"
	/*** end header inclusion ***/

namespace Util
{
	class Rect;
	class Vec2;
	class Vec3;
	class Color;
}

namespace Script
{
	class LuaEngine
	{
	private:
		LuaEngine(const LuaEngine& rhs);
		LuaEngine& operator=(const LuaEngine& rhs);

	public:
		LuaEngine();
		~LuaEngine();

		bool DoFile(const String& filename);
		bool DoFile(const char* filename);

		bool DoString(const String& script);
		bool DoString(const char* script);

		bool GetField(const char* name, int tableIndex = LUA_GLOBALSINDEX);
		bool GetField(int tableIndex);
		
		bool Push(const int& in);
		bool Push(const uint& in);
		bool Push(const String& in);
		bool Push(const char* in);
		bool Push(const bool& in);
		bool Push(const double& in);
		bool Push(const float& in);
		bool Push(const Util::Rect& in);
		bool Push(const Util::Vec2& in);
		bool Push(const Util::Vec3& in);
		bool Push(const Util::Color& in);

		bool Pop(int& out, int index = -1);
		bool Pop(uint& out, int index = -1);
		bool Pop(bool& out, int index = -1);
		bool Pop(String& out, int index = -1);
		bool Pop(double& out, int index = -1);
		bool Pop(float& out, int index = -1);
		bool Pop(Util::Rect& in, int index = -1);
		bool Pop(Util::Vec2& in, int index = -1);
		bool Pop(Util::Vec3& in, int index = -1);
		bool Pop(Util::Color& in, int index = -1);
		bool Pop();
		bool RemoveElement(int index);

		template<class T>
		bool GetValue(const char* name, T& out)
		{
			if(!GetField(name))
			{
				return false;
			}
			return Pop(out);
		}

		template<class T>
		bool GetValue(const String& name, T& out)
		{
			return GetValue(name.c_str(), out);
		}

		bool CallFunction(int arguments, int expectedResults = LUA_MULTRET);
		
		void ClearStack();
		int GetStackSize();
		int GetResultCount();
	private:
		lua_State* _lua;
		int _nresults;
		
		void HandleError(const char* name, int result);
	};
}

#endif //SCRIPT_LUAENGINE_H_