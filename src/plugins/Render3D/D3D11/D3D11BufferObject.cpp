#include "D3D11BufferObject.h"
#include "D3D11Utility.h"
#include "D3D11Context.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11BufferObject * D3D11BufferObject::Create(D3D11Context* mContext,
			const uint8_t* mBufferData, size_t mBufferSize, AccessFlag mAccess, BufferBindFlag mBind)
		{
			Ptr<D3D11BufferObject> mRet = new D3D11BufferObject(mContext);

			D3D11_BUFFER_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));
			mDesc.ByteWidth = static_cast<UINT>(mBufferSize);

			mDesc.Usage = AccessFlagToUsage(mAccess);

			if (mDesc.Usage == D3D11_USAGE_STAGING)
			{
				if (mAccess & Access_CPURead) mDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
				if (mAccess & Access_CPUWrite) mDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			}

			if (mDesc.Usage != D3D11_USAGE_STAGING)
				mDesc.BindFlags = BufferBindFlagToD3D11BindFlag(mBind);

			HRESULT hr = S_OK;
			if (mBufferData != nullptr)
			{
				D3D11_SUBRESOURCE_DATA mInitialData;
				memset(&mInitialData, 0, sizeof(mInitialData));
				mInitialData.pSysMem = mBufferData;
				hr = mContext->D3DDevice()->CreateBuffer(&mDesc, &mInitialData, &mRet->mBuffer);
			}
			else hr = mContext->D3DDevice()->CreateBuffer(&mDesc, nullptr, &mRet->mBuffer);
			RETURN_NULL_IF_FAILED(hr);

			return mRet.Detach();
		}

		const Render3DFactory * D3D11BufferObject::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}