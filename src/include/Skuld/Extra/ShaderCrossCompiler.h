#pragma once

#include "../String.h"
#include "../IDisposable.h"

namespace Skuld
{
	namespace Extra
	{
		enum ShaderBinaryFormat
		{
			ShaderBinaryFormat_DXBC,
			ShaderBinaryFormat_DXIL,
			ShaderBinaryFormat_SPIRV,
			ShaderBinaryFormat_MetalIR
		};

		class SCC_Data : public IDisposable
		{
		protected:
			virtual ~SCC_Data() {};
		public:
			virtual const uint8_t *Data() const = 0;
			virtual size_t DataSize() const = 0;
			virtual const char* ErrorOrWarning() const = 0;
		};

		SKULD_EXTRA_EXPORT SCC_Data* CompileHLSL(const char* mHLSL, ShaderBinaryFormat mFormat);
		SKULD_EXTRA_EXPORT std::string SPIRVToGLSL(const SCC_Data* mData);
		typedef SCC_Data* (ShaderCompileFunc)(const char* mHLSL, ShaderBinaryFormat mFormat);
	}
}