#pragma once

#include <Skuld/Render3D/BufferObject.h>
#include <Skuld/Render3D/Render3DContext.h>
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

			D3D11Context* mContext;
			
			friend class D3D11Context;

			static D3D11BufferObject* Create(D3D11Context* mContext,
				const uint8_t* mBufferData, size_t mBufferSize, AccessFlag mAccess, BufferBindFlag mBind);
			
			D3D11BufferObject(D3D11Context* mContext) : mContext(mContext) {}
		public:
			const Render3DFactory* GetFactory() const;
		};
	}
}