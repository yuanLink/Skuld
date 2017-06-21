#pragma once

#include "../Stream.h"

namespace Skuld
{
	namespace Script
	{
		class ScriptLoader : public IDisposable
		{
		protected:
			~ScriptLoader() {}
		public:
			static SKULD_EXPORT ScriptLoader* CreateScriptLoader(const String& type);

			virtual void LoadScript(const uint8_t* data, size_t size) = 0;
			virtual void LoadScript(const String& file) = 0;
		};
	}
}