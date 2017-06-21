#pragma once
#include "../Bitmap.h"
#include "../Stream.h"

namespace Skuld
{
	namespace Codec
	{
		enum BitmapFormat
		{
			BitmapFormat_PNG,
			BitmapFormat_ETC1,
			BitmapFormat_ICRAW,
			BitmapFormat_TGA,
			BitmapFormat_JPEG
		};

		SKULD_EXPORT Bitmap* DecodeBitmapFromStream(Stream* stream, uint32_t mBitmapIndex = 0);
		SKULD_EXPORT void EncodeBitmapFromStream(Stream* stream, const Bitmap* bmp, BitmapFormat format);
		SKULD_EXPORT BitmapFormat BitmapFormatFromString(const String& mStr);
	};
}