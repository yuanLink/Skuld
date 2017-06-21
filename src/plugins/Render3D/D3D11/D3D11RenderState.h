#pragma once

#include <Skuld/Render3D/RenderState.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11RenderState :public RenderState
		{
		protected:
			virtual ~D3D11RenderState() {}

			const D3D11Factory* mFactory;

			CComPtr<ID3D11RasterizerState> mRasterizerState;
			CComPtr<ID3D11DepthStencilState> mDepthStencilState;
			CComPtr<ID3D11BlendState> mBlendState;

			friend class D3D11Context;
			static D3D11RenderState* Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory);

			D3D11RenderState(CComPtr<ID3D11RasterizerState> mRasterizerState,
				CComPtr<ID3D11DepthStencilState> mDepthStencilState,
				CComPtr<ID3D11BlendState> mBlendState,
				const D3D11Factory* mFactory
			) :
				mRasterizerState(mRasterizerState),
				mDepthStencilState(mDepthStencilState),
				mBlendState(mBlendState),
				mFactory(mFactory)
			{}

		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}