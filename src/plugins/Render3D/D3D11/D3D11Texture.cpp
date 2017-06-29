#include "D3D11Texture.h"
#include "D3D11Context.h"

namespace Skuld
{
	namespace Render3D
	{
		const Render3DFactory * D3D11Texture::GetFactory() const
		{
			return mContext->GetFactory();
		}
	}
}