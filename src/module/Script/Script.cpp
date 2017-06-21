#include <Skuld/Script/Script.h>
#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Script
	{
#ifdef SKULD_ENABLE_MSIL
		ScriptLoader* CreateMSILLoader();
#endif
#ifdef SKULD_ENABLE_LUA
		ScriptLoader* CreateLuaLoader();
#endif
#ifdef SKULD_ENABLE_JAVASCRIPT
		ScriptLoader* CreateJavaScriptLoader();
#endif

		ScriptLoader * ScriptLoader::CreateScriptLoader(const String & type)
		{
			String lower = type.ToLower();
#ifdef SKULD_ENABLE_MSIL
			if (lower == "clr" || lower == "msil" || lower == "mono" || lower == ".net")
				return CreateMSILLoader();
#endif
#ifdef SKULD_ENABLE_LUA
			if (lower == "lua")
				return CreateLuaLoader();
#endif
#ifdef SKULD_ENABLE_JAVASCRIPT
			if (lower == "javascript" || lower == "js" || lower == "ecmascript")
				return CreateJavaScriptLoader();
#endif

			throw Exception("未知的ScriptLoader");
			return nullptr;
		}
	}
}