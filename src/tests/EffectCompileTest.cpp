#include <Skuld/Core/Effect.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>

using namespace Skuld::Core;
using namespace Skuld;

int main()
{
	{
		Ptr<Effect> Eff = Effect::CompileFromString("", R"({"Ð§¹û1":{"VertexShader":"vs_main","PixelShader":"ps_main",
				"VertexLayout":{"b":{"Type":"int","Size":4, "Slot":1},"a":{"Type":"float","Size":4, "Slot":0}}}})");
		Eff->Save(MakePtr(FileStream::Open("effectc.efct", FileMode::Create)));
	}
	{
		Ptr<Effect> Eff = Effect::LoadFromStream(MakePtr(FileStream::Open("effectc.efct", FileMode::Open)));
	}
	return 0;
}