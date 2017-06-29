#pragma once
#include <Skuld/Render3D/ShaderObject.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11ShaderObject : public ShaderObject
		{
		protected:
			virtual ~D3D11ShaderObject() {}
			friend class D3D11Context;
			static D3D11ShaderObject* CreateD3D11Shader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType, 
				D3D11Context* mContext);

			CComPtr<ID3D11DeviceChild> mShader;
			std::vector<uint8_t> mCode;
			ShaderType mType;
			D3D11Context* mContext;

			D3D11ShaderObject(const uint8_t * mCode, size_t mCodeSize, ShaderType mType, D3D11Context* mContext);
		public:
			virtual const uint8_t* GetCode() const;
			virtual const size_t GetCodeSize() const;
			virtual ShaderType GetShaderType() const;
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}