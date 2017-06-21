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

			std::string mEntry;
			std::string mProfile;

			virtual Extra::ShaderBinaryFormat Format() = 0;
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst) = 0;
		};
	}
}