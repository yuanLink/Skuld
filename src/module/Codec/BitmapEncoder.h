#pragma once

#include <Skuld/Codec/BitmapCodec.h>
#include <memory>

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoder
		{
		public:
			virtual BitmapFormat Format() = 0;
			virtual void Encode(const Bitmap* bmp, Stream* stream) = 0;
		};
	}
}