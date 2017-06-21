#pragma once
#include <Skuld/Ptr.hpp>
#include <Skuld/Render3D/Render3DContext.h>
#include <map>
#include <Skuld/Render3D/Texture.h>
#include <Skuld/Bitmap.h>

namespace Skuld
{
	namespace Core
	{
		class TextureManager
		{
		protected:
			Ptr<Render3D::Render3DContext> mContext;
			std::map<String, Ptr<Render3D::Texture> > mTexture;
		public:
			TextureManager(Ptr<Render3D::Render3DContext> mContext) : mContext(mContext) {}

			void AddTexture2D(const String& mPath, const Bitmap* mBitmap);
		};
	}
}