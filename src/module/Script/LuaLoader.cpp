#include <Skuld/Script/Script.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

namespace Skuld
{
	namespace Script
	{
		class LuaLoader : public ScriptLoader
		{
		protected:
			lua_State *mState;
			~LuaLoader();
		public:
			LuaLoader(lua_State* mState) : mState(mState) {}
			void LoadScript(const uint8_t* data, size_t size) {}
			void LoadScript(const String& file) {}
		};

		ScriptLoader* CreateLuaLoader()
		{
			lua_State* mState = luaL_newstate();
			return new LuaLoader(mState);
		}

		LuaLoader::~LuaLoader()
		{
			lua_close(mState);
		}
	}
}