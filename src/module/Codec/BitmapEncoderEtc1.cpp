#include <memory>
#include "BitmapEncoder.h"
#include <etc1.h>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoderEtc1 : public BitmapEncoder
		{
			BitmapFormat Format() { return BitmapFormat_ETC1; }
			void Encode(const Bitmap* bmp, Stream * stream);
		};

		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderEtc1() {
			return std::make_shared<BitmapEncoderEtc1>();
		}

		void BitmapEncoderEtc1::Encode(const Bitmap* bmp, Stream * stream)
		{
			Ptr<Bitmap> mBmp(bmp->Convert(PixelFormat_RGB_888));
			uint32_t mEncodedSize = etc1_get_encoded_data_size(bmp->GetWidth(), bmp->GetHeight());
			uint8_t mHeader[ETC_PKM_HEADER_SIZE];
			etc1_pkm_format_header(mHeader, bmp->GetWidth(), bmp->GetHeight());

			std::vector<uint8_t> mEncoded(mEncodedSize);

			etc1_encode_image(mBmp->GetPixels(), bmp->GetWidth(), bmp->GetHeight(),
				3, 3 * bmp->GetWidth(), mEncoded.data());

			stream->Write(mHeader, ETC_PKM_HEADER_SIZE);
			stream->Write(mEncoded.data(), mEncodedSize);
		}
	}
}