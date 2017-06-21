#include <Skuld/Script/Script.h>

namespace Skuld
{
	namespace Script
	{
		class JavaScriptLoader : public ScriptLoader
		{
		protected:
			~JavaScriptLoader();
		public:
			void LoadScript(const uint8_t* data, size_t size) {}
			void LoadScript(const String& file) {}
		};

		ScriptLoader* CreateJavaScriptLoader()
		{
			return new JavaScriptLoader();
		}

		JavaScriptLoader::~JavaScriptLoader()
		{
		}
	}
}