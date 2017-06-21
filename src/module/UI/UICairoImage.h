#pragma once
#include <Skuld/UI/UIImage.h>
#include <cairo.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/Bitmap.h>

namespace Skuld
{
	namespace UI
	{
		class UICairoContext;
		class UICairoImage : public UIImage
		{
		protected:
			friend class UICairoContext;
			cairo_surface_t *mSurface = nullptr;
			~UICairoImage();
			UICairoContext *mContext;
			Ptr<Bitmap> mPixels;
		public:
			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			UICairoImage(cairo_surface_t *mSurface, UICairoContext* mContext, Bitmap* mPixels) :
				mSurface(mSurface), mContext(mContext), mPixels(mPixels) {}
			UIContext* GetContext() const;
		};
	}
}