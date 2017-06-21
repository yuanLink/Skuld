#pragma once
#include <Skuld/Render3D/InputLayout.h>
#include "D3D11Shader.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11InputLayout : public InputLayout
		{
		protected:
			virtual ~D3D11InputLayout() {}

			CComPtr<ID3D11InputLayout> mInputLayout;
			const D3D11Factory* mFactory;

			friend class D3D11Context;

			static D3D11InputLayout* CreateD3D11InputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize,
				Shader* mShader, CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory);

			D3D11InputLayout(CComPtr<ID3D11InputLayout> mInputLayout, const D3D11Factory* mFactory) :
				mInputLayout(mInputLayout), mFactory(mFactory) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}