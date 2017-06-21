#pragma once

#include <Skuld/Codec/AudioCodec.h>
#include <memory>

namespace Skuld
{
	namespace Codec
	{
		class AudioDecoder
		{
		public:
			virtual bool Test(Stream* stream);
			virtual AudioDecodeStream* Open(Stream* mStream) = 0;
		};
	}
}