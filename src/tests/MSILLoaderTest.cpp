#include <Skuld/Script/Script.h>
#include <Skuld/Ptr.hpp>

using namespace Skuld;
using namespace Skuld::Script;

int main()
{
	Ptr<ScriptLoader> mLoader(ScriptLoader::CreateScriptLoader("msil"));

	mLoader->LoadScript("haha.dll");
	return 0;
}