#include <boost/program_options.hpp>
#include <Skuld/Core/Effect.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>
#include <Skuld/Exception.h>

using namespace boost::program_options;
using namespace Skuld;
using namespace Skuld::Core;

int main(int argc, char** argv)
{
	options_description mDesc("effectc");

	mDesc.add_options()
		("help,h", "Show help.")
		("hlsl", value<String>(), "HLSL File")
		("effect-script", value<String>(), "Effect Script JSON")
		("output,o", value<String>(), "Output Effect file");
	variables_map mVM;
	store(command_line_parser(argc, argv).options(mDesc).run(), mVM);

	if (mVM.count("help")) std::cout << mDesc << std::endl;
	else
	{
		if (mVM.count("hlsl") == 0 || mVM.count("effect-script") == 0 || mVM.count("output") == 0)
			std::cout << mDesc << std::endl;
		else
		{
			Ptr<DynamicLibrary> mSCC = DynamicLibrary::OpenDynamicLibrary("ShaderCrossCompiler" SKULD_DL_POSTFIX);

			std::function<Skuld::Extra::ShaderCompileFunc> mEntry =
				mSCC->GetSymbol<Skuld::Extra::ShaderCompileFunc>("Skuld_CompileHLSL");

			if (mEntry == nullptr)
				mEntry = mSCC->GetSymbol<Skuld::Extra::ShaderCompileFunc>("_Skuld_CompileHLSL");

			if (mEntry == nullptr)
			{
				std::cout << "Cannot load ShaderCrossCompiler" SKULD_DL_POSTFIX << std::endl;
				return -1;
			}

			LoadShaderCrossCompiler(mEntry);

			Ptr<Effect> mEffect;
			try
			{
				mEffect = Effect::CompileFromFile(mVM["hlsl"].as<String>(), mVM["effect-script"].as<String>());
			}
			catch (Exception& e)
			{
				std::cout << e.Message() << std::endl;
				return -2;
			}
			Ptr<FileStream> mOutput = FileStream::Open(mVM["output"].as<String>(), Create);

			if (mOutput == nullptr)
			{
				std::cout << "Cannot open output file" << std::endl;
				return -1;
			}

			mEffect->Save(mOutput);
		}
	}

	return 0;
}