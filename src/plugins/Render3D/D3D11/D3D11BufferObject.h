#pragma once

#include <Skuld/Render3D/BufferObject.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11BufferObject : public BufferObject
		{
		protected:
			virtual ~D3D11BufferObject() {}

			CComPtr<ID3D11Buffer> mBuffer;

			const D3D11Factory* mFactory;
			
			friend class D3D11Context;

			static D3D11BufferObject* Create(CComPtr<ID3D11Device> mDevice, const D3D11Factory* mFactory,
				uint8_t* mBufferData, size_t mBufferSize);
			
			D3D11BufferObject(const D3D11Factory* mFactory, CComPtr<ID3D11Buffer> mBuffer) :
				mFactory(mFactory), mBuffer(mBuffer) {}
		public:
			const Render3DFactory* GetFactory() const;
		};
	}
}