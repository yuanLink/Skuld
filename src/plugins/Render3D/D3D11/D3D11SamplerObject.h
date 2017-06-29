#pragma once

#include <Skuld/Render3D/SamplerObject.h>
#include "D3D11Factory.h"

namespace Skuld
{
	namespace Render3D
	{
		class D3D11SamplerObject : public SamplerObject
		{
		protected:
			virtual ~D3D11SamplerObject() {}

			friend class D3D11Context;
			CComPtr<ID3D11SamplerState> mSampler;
			D3D11Context* mContext;

			static D3D11SamplerObject* Create(D3D11Context* mContext);

			D3D11SamplerObject(D3D11Context* mContext) :
				mContext(mContext) {}
		public:
			virtual const Render3DFactory* GetFactory() const;
		};
	}
}