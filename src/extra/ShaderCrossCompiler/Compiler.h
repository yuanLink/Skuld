#pragma once

#include <Skuld/Extra/ShaderCrossCompiler.h>
#include <Skuld/Exception.h>
#include <vector>
#include <string>
#include <memory>

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		class Compiler
		{
		public:
			std::string mWarning;

			virtual Extra::ShaderBinaryFormat Format() = 0;
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst,
				Render3D::ShaderType mProfile, const char* mMain) = 0;
		};
	}
}