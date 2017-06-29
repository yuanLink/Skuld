#pragma once

#include "../String.h"
#include "../IDisposable.h"
#include "../Render3D/ShaderObject.h"

namespace Skuld
{
	namespace Extra
	{
		enum ShaderBinaryFormat
		{
			ShaderBinaryFormat_DXBC,
			ShaderBinaryFormat_DXIL,
			ShaderBinaryFormat_SPIRV,
			ShaderBinaryFormat_GLSL,
			ShaderBinaryFormat_Metal
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

		SKULD_EXTRA_EXPORT SCC_Data* CompileHLSL(const char* mHLSL, ShaderBinaryFormat mFormat,
			Render3D::ShaderType mProfile, const char* mMain);
		typedef SCC_Data* (ShaderCompileFunc)(const char* mHLSL, ShaderBinaryFormat mFormat,
			Render3D::ShaderType mProfile, const char* mMain);
	}
}