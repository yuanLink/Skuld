#include "D3D11RenderState.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11RenderState * D3D11RenderState::Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory)
		{
			CComPtr<ID3D11BlendState> mBlendState;
			CComPtr<ID3D11RasterizerState> mRasterizerState;
			CComPtr<ID3D11DepthStencilState> mDepthStencilState;

			D3D11_RASTERIZER_DESC mRasterizerDesc;
			D3D11_BLEND_DESC mBlendDesc;
			D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;

			mDevice->CreateRasterizerState(&mRasterizerDesc, &mRasterizerState);
			mDevice->CreateDepthStencilState(&mDepthStencilDesc, &mDepthStencilState);
			mDevice->CreateBlendState(&mBlendDesc, &mBlendState);

			return new D3D11RenderState(mRasterizerState, mDepthStencilState, mBlendState, mFactory);
		}

		const Render3DFactory * D3D11RenderState::GetFactory() const
		{
			return mFactory;
		}
	}
}