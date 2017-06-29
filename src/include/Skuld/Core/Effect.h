#pragma once

#include "../Stream.h"
#include "../Render3D/InputLayout.h"
#include "../Extra/ShaderCrossCompiler.h"

namespace Skuld
{
	namespace Core
	{
		class Effect : public IDisposable
		{
		protected:
			virtual ~Effect() {}
		public:
			static SKULD_EXPORT Effect* LoadFromStream(Stream* stream);

			static SKULD_EXPORT Effect* CompileFromString(const String& mHLSL, const String& mEffectScript,
				Extra::ShaderBinaryFormat mFormat);
			static SKULD_EXPORT Effect* CompileFromFile(const String& mHLSLFile, const String& mEffectScriptFile,
				Extra::ShaderBinaryFormat mFormat);
			static SKULD_EXPORT Effect* CompileFromStream(Stream* mHLSL, Stream* mEffectScript,
				Extra::ShaderBinaryFormat mFormat);

			virtual CheckFlags GetShaderType() const = 0;

			virtual void Save(Stream* mStream) const = 0;

			virtual const uint8_t* GetShader(const String& str) const = 0;
			virtual size_t GetShaderSize(const String& str) const = 0;

			virtual size_t GetShaderCount() const = 0;
			virtual const Render3D::ShaderInputLayoutAttri* GetInputLayoutAttri(size_t mIndex) const = 0;
			virtual size_t GetEffectCount() const = 0;
			virtual String GetVertexShader(size_t mIndex) const = 0;
			virtual String GetPixelShader(size_t mIndex) const = 0;
			virtual String GetGeometryShader(size_t mIndex) const = 0;
			virtual String GetHullShader(size_t mIndex) const = 0;
			virtual String GetDomainShader(size_t mIndex) const = 0;
			virtual bool HasGeometryShader(size_t mIndex) const = 0;
			virtual bool HasHullShader(size_t mIndex) const = 0;
			virtual bool HasDomainShader(size_t mIndex) const = 0;
			virtual String GetEffectName(size_t mIndex) const = 0;
			virtual size_t GetInputLayoutAttriCount(size_t mIndex) const = 0;
		};
	}
}