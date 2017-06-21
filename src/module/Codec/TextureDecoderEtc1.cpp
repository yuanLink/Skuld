#include <etc1.h>
#include "TextureDecoder.h"

namespace Skuld
{
	namespace Codec
	{
		class TextureDecoderEtc1 : public TextureDecoder
		{

		};

		void Decode(Stream* mStream)
		{
			uint8_t flag__[ETC_PKM_HEADER_SIZE];
			mStream->Read(flag__, ETC_PKM_HEADER_SIZE);
			uint32_t width = etc1_pkm_get_width(flag__), height = etc1_pkm_get_height(flag__);

			uint32_t buffer_size = etc1_get_encoded_data_size(width, height);

			std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(buffer_size);
			if (mStream->Read(buffer.get(), buffer_size) == buffer_size)
			{

			}
		}
	}
}