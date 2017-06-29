#include <boost/program_options.hpp>
#include <Skuld/Core/Effect.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>
#include <Skuld/Exception.h>

using namespace boost::program_options;
using namespace Skuld;
using namespace Skuld::Core;

Extra::ShaderBinaryFormat selectFormat(const String& str)
{
	String l = str.ToLower();
	if (l == "dxbc") return Extra::ShaderBinaryFormat_DXBC;
	else if (l == "dxil") return Extra::ShaderBinaryFormat_DXIL;
	else if (l == "spirv") return Extra::ShaderBinaryFormat_SPIRV;
	else if (l == "glsl") return Extra::ShaderBinaryFormat_GLSL;
	else if (l == "metal") return Extra::ShaderBinaryFormat_Metal;
	return Extra::ShaderBinaryFormat_DXBC;
}

int main(int argc, char** argv)
{
	options_description mDesc("effectc");

	mDesc.add_options()
		("help,h", "Show help.")
		("hlsl", value<String>(), "HLSL File")
		("effect-script", value<String>(), "Effect Script JSON")
		("output,o", value<String>(), "Output Effect file")
		("format,f", value<String>(), "Output format(dxbc, dxil, spirv, glsl, metal)");
	variables_map mVM;
	store(command_line_parser(argc, argv).options(mDesc).run(), mVM);

	if (mVM.count("help")) std::cout << mDesc << std::endl;
	else
	{
		if (mVM.count("hlsl") == 0 || mVM.count("effect-script") == 0 || mVM.count("output") == 0)
			std::cout << mDesc << std::endl;
		else
		{
			Extra::ShaderBinaryFormat mFormat;
			if (mVM.count("format") == 0) mFormat = Extra::ShaderBinaryFormat_DXBC;
			else mFormat = selectFormat(mVM["format"].as<String>());

			Ptr<Effect> mEffect;
			try
			{
				mEffect = Effect::CompileFromFile(mVM["hlsl"].as<String>(), mVM["effect-script"].as<String>(), mFormat);
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