#include "UICairoImage.h"
#include "UICairoContext.h"

namespace Skuld
{
	namespace UI
	{
		UICairoImage::~UICairoImage() {
			cairo_surface_destroy(mSurface);
		}

		uint32_t UICairoImage::GetHeight() const
		{
			return cairo_image_surface_get_height(mSurface);
		}

		UIContext * UICairoImage::GetContext() const
		{
			return this->mContext;
		}

		uint32_t UICairoImage::GetWidth() const
		{
			return cairo_image_surface_get_width(mSurface);
		}
	}
}