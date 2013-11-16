#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include <Engine/Defines.h>
	/*** extra headers if needed (alphabetically ordered) ***/
#include <Services/Script/LuaInclude.h>
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
	class Engine
	{
	private:
		Engine(const Engine& rhs);
		Engine& operator=(const Engine& rhs);

	public:
		Engine();
		~Engine();

		bool DoFile(const std::string& filename);
		bool DoFile(const char* filename);

		bool DoString(const std::string& script);
		bool DoString(const char* script);

		bool GetField(const char* name, int tableIndex = LUA_GLOBALSINDEX);
		bool GetField(int tableIndex);
		
		bool Push(const int& in);
		bool Push(const uint32_t& in);
		bool Push(const std::string& in);
		bool Push(const char* in);
		bool Push(const bool& in);
		bool Push(const double& in);
		bool Push(const float& in);
		bool Push(const Util::Rect& in);
		bool Push(const Util::Vec2& in);
		bool Push(const Util::Vec3& in);
		bool Push(const Util::Color& in);

		bool Pop(int& out, int index = -1);
		bool Pop(uint32_t& out, int index = -1);
		bool Pop(bool& out, int index = -1);
		bool Pop(std::string& out, int index = -1);
		bool Pop(double& out, int index = -1);
		bool Pop(float& out, int index = -1);
		bool Pop(Util::Rect& in, int index = -1);
		bool Pop(Util::Vec2& in, int index = -1);
		bool Pop(Util::Vec3& in, int index = -1);
		bool Pop(Util::Color& in, int index = -1);
		bool Pop();
		bool RemoveElement(int index);

		bool GetObjLength(int& out, int index = -1);

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
		bool GetValue(const std::string& name, T& out)
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