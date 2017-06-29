#include "D3D11RenderState.h"
#include "D3D11Context.h"

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11RenderStateHelper
		{
		public:
			static inline HRESULT CreateRasterizerState(D3D11Context* mContext, D3D11RenderState* mRet)
			{
#ifdef __d3d11_3_h__
				if (mContext->D3DDevice3())
				{
					D3D11_RASTERIZER_DESC2 mRasterizerDesc2;
					memset(&mRasterizerDesc2, 0, sizeof(mRasterizerDesc2));

					HRESULT hr3 = mContext->D3DDevice3()->CreateRasterizerState2(&mRasterizerDesc2, &mRet->mRasterizerState2);

					if (!FAILED(hr3))
					{
						mRet->mRasterizerState2->QueryInterface(&mRet->mRasterizerState1);
						mRet->mRasterizerState2->QueryInterface(&mRet->mRasterizerState);
					}
					return hr3;
				}
#endif

#ifdef __d3d11_1_h__
				if (mContext->D3DDevice1())
				{
					D3D11_RASTERIZER_DESC1 mRasterizerDesc1;
					memset(&mRasterizerDesc1, 0, sizeof(mRasterizerDesc1));
					HRESULT hr1 = mContext->D3DDevice1()->CreateRasterizerState1(&mRasterizerDesc1, &mRet->mRasterizerState1);
					if (!FAILED(hr1))
						mRet->mRasterizerState1->QueryInterface(&mRet->mRasterizerState);
					return hr1;
				}
#endif

				D3D11_RASTERIZER_DESC mRasterizerDesc;
				memset(&mRasterizerDesc, 0, sizeof(mRasterizerDesc));
				return mContext->D3DDevice()->CreateRasterizerState(&mRasterizerDesc, &mRet->mRasterizerState);
			}

			static inline HRESULT CreateBlendState(D3D11Context* mContext, D3D11RenderState* mRet)
			{
				D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;
				memset(&mDepthStencilDesc, 0, sizeof(mDepthStencilDesc));
				return mContext->D3DDevice()->CreateDepthStencilState(&mDepthStencilDesc, &mRet->mDepthStencilState);
			}

			static inline HRESULT CreateDepthStencilState(D3D11Context* mContext, D3D11RenderState* mRet)
			{
#ifdef __d3d11_1_h__
				if (mContext->D3DDevice1())
				{
					D3D11_BLEND_DESC1 mBlendDesc1;
					memset(&mBlendDesc1, 0, sizeof(mBlendDesc1));
					HRESULT hr1 = mContext->D3DDevice1()->CreateBlendState1(&mBlendDesc1, &mRet->mBlendState1);
					mRet->mBlendState1->QueryInterface(&mRet->mBlendState);
					return hr1;
				}
#endif
				D3D11_BLEND_DESC mBlendDesc;
				memset(&mBlendDesc, 0, sizeof(mBlendDesc));
				return mContext->D3DDevice()->CreateBlendState(&mBlendDesc, &mRet->mBlendState);
			}
		};

		D3D11RenderState * D3D11RenderState::Create(D3D11Context* mContext)
		{
			Ptr<D3D11RenderState> mRet = new D3D11RenderState(mContext);

			RETURN_NULL_IF_FAILED(D3D11RenderStateHelper::CreateRasterizerState(mContext, mRet));
			RETURN_NULL_IF_FAILED(D3D11RenderStateHelper::CreateBlendState(mContext, mRet));
			RETURN_NULL_IF_FAILED(D3D11RenderStateHelper::CreateDepthStencilState(mContext, mRet));

			return mRet.Detach();
		}

		const Render3DFactory * D3D11RenderState::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}