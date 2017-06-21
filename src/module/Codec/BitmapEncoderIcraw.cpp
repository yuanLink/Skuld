#include <memory>
#include "BitmapEncoder.h"
#include "BitmapDecoder.h"

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoderIcraw : public BitmapEncoder
		{
			BitmapFormat Format() { return BitmapFormat_ICRAW; }
			void Encode(const Bitmap* bmp, Stream * stream);
		};

		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderIcraw() {
			return std::make_shared<BitmapEncoderIcraw>();
		}

		void BitmapEncoderIcraw::Encode(const Bitmap* bmp, Stream * stream)
		{
			icraw_header header;
			header.format = bmp->GetPixelFormat();
			header.height = bmp->GetHeight();
			header.width = bmp->GetWidth();
			memcpy(header.flag, "ICR ", 4);
			stream->Write(&header, sizeof(icraw_header));

			size_t buffer_size = (header.width * header.height * PixelFormatDepth(header.format) + 7) / 8;

			stream->Write(bmp->GetPixels(), buffer_size);
		}
	}
}