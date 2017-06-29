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

			D3D11Context* mContext;

			CComPtr<ID3D11RasterizerState> mRasterizerState;
			CComPtr<ID3D11DepthStencilState> mDepthStencilState;
			CComPtr<ID3D11BlendState> mBlendState;

#ifdef __d3d11_1_h__
			CComPtr<ID3D11RasterizerState1> mRasterizerState1;
			CComPtr<ID3D11BlendState1> mBlendState1;
#endif
#ifdef __d3d11_3_h__
			CComPtr<ID3D11RasterizerState2> mRasterizerState2;
#endif

			friend class D3D11Context;
			friend class D3D11RenderStateHelper;
			static D3D11RenderState* Create(D3D11Context* mContext);

			D3D11RenderState(D3D11Context* mContext) : mContext(mContext) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}