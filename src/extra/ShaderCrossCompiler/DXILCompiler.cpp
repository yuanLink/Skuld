#include "Compiler.h"

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		class DXILCompiler : public Compiler
		{
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_DXIL; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst,
				Render3D::ShaderType mProfile, const char* mMain);
		};

		bool DXILCompiler::Compile(const char * mSrc, std::vector<uint8_t>& mDst,
			Render3D::ShaderType mProfile, const char * mMain)
		{
			return false;
		}

		std::shared_ptr<Compiler> CreateDXILCompiler() {
			return std::make_shared<DXILCompiler>();
		}
	}
}