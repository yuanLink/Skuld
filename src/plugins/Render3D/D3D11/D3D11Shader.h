#pragma once
#include <Skuld/Render3D/Shader.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Shader : public Shader
		{
		protected:
			virtual ~D3D11Shader() {}
			friend class D3D11Context;
			static D3D11Shader* CreateD3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType, 
				ID3D11Device* mDevice, const D3D11Factory* mFactory);

			CComPtr<ID3D11DeviceChild> mShader;
			std::vector<uint8_t> mCode;
			ShaderType mType;
			const D3D11Factory* mFactory;

			D3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType,
				CComPtr<ID3D11DeviceChild> mShader, const D3D11Factory* mFactory);
		public:
			virtual const uint8_t* GetCode() const;
			virtual const size_t GetCodeSize() const;
			virtual ShaderType GetShaderType() const;
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}