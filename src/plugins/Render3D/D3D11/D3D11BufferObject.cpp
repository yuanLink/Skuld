#include "D3D11BufferObject.h"

namespace Skuld
{
	namespace Render3D
	{
		D3D11BufferObject * D3D11BufferObject::Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory * mFactory,
			uint8_t* mBufferData, size_t mBufferSize)
		{
			CComPtr<ID3D11Buffer> mBuffer;

			D3D11_BUFFER_DESC mDesc;
			memset(&mDesc, 0, sizeof(mDesc));

			if (mBufferData != nullptr)
			{
				D3D11_SUBRESOURCE_DATA mInitialData;
				memset(&mInitialData, 0, sizeof(mInitialData));
				mInitialData.pSysMem = mBufferData;
				mDevice->CreateBuffer(&mDesc, &mInitialData, &mBuffer);
			}
			else mDevice->CreateBuffer(&mDesc, nullptr, &mBuffer);

			return new D3D11BufferObject(mFactory, mBuffer);
		}

		const Render3DFactory * D3D11BufferObject::GetFactory() const
		{
			return mFactory;
		}
	}
}