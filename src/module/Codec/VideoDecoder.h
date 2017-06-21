#pragma once

namespace Skuld
{
	namespace Codec
	{
		class VideoDecoder
		{
		public:
			virtual bool Test(Stream* mStream);
		};
	}
}