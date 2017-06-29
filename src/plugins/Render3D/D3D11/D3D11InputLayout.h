#pragma once
#include <Skuld/Render3D/InputLayout.h>
#include "D3D11ShaderObject.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11InputLayout : public InputLayout
		{
		protected:
			virtual ~D3D11InputLayout() {}

			CComPtr<ID3D11InputLayout> mInputLayout;
			D3D11Context* mContext;

			friend class D3D11Context;

			static D3D11InputLayout* CreateD3D11InputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize,
				ShaderObject* mShader, D3D11Context* mContext);

			D3D11InputLayout(D3D11Context* mContext) : mContext(mContext) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}