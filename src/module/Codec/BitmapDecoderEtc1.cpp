#include <etc1.h>
#include "BitmapDecoder.h"
#include <memory>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Codec
	{
		class BitmapDecoderEtc1 : public BitmapDecoder
		{
		public:
			virtual Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex);
			virtual bool Test(Stream* stream);
		};

		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderEtc1() {
			return std::make_shared<BitmapDecoderEtc1>();
		}

		Bitmap * BitmapDecoderEtc1::Decode(Stream * stream, uint32_t mBitmapIndex)
		{
			uint8_t flag__[ETC_PKM_HEADER_SIZE];
			stream->Read(flag__, ETC_PKM_HEADER_SIZE);
			uint32_t width = etc1_pkm_get_width(flag__), height = etc1_pkm_get_height(flag__);

			uint32_t buffer_size = etc1_get_encoded_data_size(width, height);

			std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(buffer_size);
			if (stream->Read(buffer.get(), buffer_size) == buffer_size)
			{
				Ptr<Bitmap> ret = Bitmap::CreateBitmap(width, height, PixelFormat_RGB_888);
				etc1_decode_image(buffer.get(), ret->GetPixels(), width, height, 3, 3 * width);
				
				return ret.Detach();
			}
			return nullptr;
		}

		bool BitmapDecoderEtc1::Test(Stream * stream)
		{
			uint8_t flag__[ETC_PKM_HEADER_SIZE];
			if (stream->Peek(flag__, ETC_PKM_HEADER_SIZE) == ETC_PKM_HEADER_SIZE)
				return etc1_pkm_is_valid(flag__) > 0;
			return false;
		}
	}
}