#include <memory>
#include "BitmapDecoder.h"
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Codec
	{
		class BitmapDecoderIcraw : public BitmapDecoder
		{
			Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex);
			bool Test(Stream* stream);
		};

		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderIcraw() {
			return std::make_shared<BitmapDecoderIcraw>();
		}

		Bitmap * BitmapDecoderIcraw::Decode(Stream * stream, uint32_t mBitmapIndex)
		{
			icraw_header header;
			if (stream->Read(&header, sizeof(icraw_header)) == sizeof(icraw_header))
			{
				Ptr<Bitmap> ret = Bitmap::CreateBitmap(header.width, header.height, header.format);
				size_t buffer_size = (header.width * header.height * PixelFormatDepth(header.format) + 7) / 8;

				if (stream->Read(ret->GetPixels(), buffer_size) == buffer_size)
					return ret.Detach();
				else return nullptr;
			}
			else return nullptr;
		}

		bool BitmapDecoderIcraw::Test(Stream * stream)
		{
			char fl[4];
			if (stream->Peek(fl, 4) == 4)
				return (memcmp(fl, "ICR ", 4) == 0);
			else return false;
		}
	}
}