#include "D3D11Texture.h"

namespace Skuld
{
	namespace Render3D
	{
		const Render3DFactory * D3D11Texture::GetFactory() const
		{
			return mFactory;
		}
	}
}