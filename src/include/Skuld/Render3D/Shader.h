#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		enum ShaderType
		{
			ShaderType_VertexShader,
			ShaderType_PixelShader,
			ShaderType_GeometryShader,
			ShaderType_HullShader,
			ShaderType_DomainShader,
			ShaderType_ComputeShader,
		};

		class Shader : public IDisposable
		{
		protected:
			virtual ~Shader() {}
		public:
			virtual const uint8_t* GetCode() const = 0;
			virtual const size_t GetCodeSize() const = 0;
			virtual ShaderType GetShaderType() const = 0;
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}