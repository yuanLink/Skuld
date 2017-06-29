#include <Skuld/Core/Engine3D.h>
#include <Skuld/Ptr.hpp>

using namespace Skuld::UI;
using namespace Skuld::Core;
using namespace Skuld;

#if defined(_DEBUG)
#define CONF "Debug"
#else
#define CONF "Release"
#endif

int main()
{
	SkuldEngine* mSkuldEngine = CreateEngine();
	const DynamicLibrary* mPlugin = LoadPlugin(mSkuldEngine, SKULD_BUILD_DIR "/" CONF "/plugin/Render3D_D3D11.dll");

	{
		Ptr<Displayable> mDis = Displayable::Create(300, 300);

		Ptr<Engine3D> mEng = Engine3D::Create3DEngine(mSkuldEngine, mDis);
		Ptr<Effect> mEffect = Effect::CompileFromFile(
			SKULD_BUILD_DIR "/../../resources/HLSL.hlsl",
			SKULD_BUILD_DIR "/../../resources/EffectScript.json",
			Extra::ShaderBinaryFormat_DXBC
		);
		mEng->LoadEffect(mEffect);
		
		while (!mDis->MainLoop());
	}

	UnloadPlugin(mSkuldEngine, mPlugin);
	DestroyEngine(mSkuldEngine);
	return 0;
}