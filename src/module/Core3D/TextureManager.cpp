#include "TextureManager.h"

using namespace Skuld::Render3D;

namespace Skuld
{
	namespace Core
	{
		void TextureManager::AddTexture2D(const String& mPath, const Bitmap * mBitmap)
		{
			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			this->mTexture[mPath] = this->mContext->CreateTexture2D(
				mBitmap->GetPixels(),
				mBitmap->GetWidth(),
				mBitmap->GetHeight(),
				mBitmap->GetPixelFormat(),
				Access_GPURead | Access_GPUWrite,
				TextureBind_ShaderResource
			);
		}
	}
}