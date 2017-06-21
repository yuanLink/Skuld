#pragma once

#include "IDisposable.h"
#include "PixelFormat.h"

namespace Skuld
{
	class Bitmap : public IDisposable
	{
	protected:
		virtual ~Bitmap() {}
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual PixelFormat GetPixelFormat() const = 0;

		virtual const uint8_t *GetPixels() const = 0;
		virtual uint8_t *GetPixels() = 0;

		virtual Bitmap* Convert(PixelFormat format) const = 0;
		virtual Bitmap* Clone() const = 0;

		SKULD_EXPORT static Bitmap* CreateBitmap(uint32_t width, uint32_t height, PixelFormat format);
	};
}