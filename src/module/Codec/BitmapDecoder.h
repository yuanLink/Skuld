#pragma once

#include <Skuld/Codec/BitmapCodec.h>

namespace Skuld
{
	namespace Codec
	{
#pragma pack(1)
		struct icraw_header
		{
			char flag[4];
			uint32_t height;
			uint32_t width;
			PixelFormat format;
			char reversed[32];
		};

#define TGA_NO_COMPRESS 2
#define TGA_RLE 10

		struct tga_header
		{
			uint8_t tImageInfoLength;
			uint8_t tIsColorTableExist;
			uint8_t tCompression;
			struct
			{
				uint16_t cEntryIndex;
				uint16_t cColorCount;
				uint8_t cBits;
			} tColorInfo;
			struct
			{
				int16_t iX;
				int16_t iY;
				int16_t iWidth;
				int16_t iHeight;
				uint8_t iBits;
				uint8_t iImageDescribedByte;
			} tImageParams;
		};
#pragma pack()

		class BitmapDecoder
		{
		public:
			virtual Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex) = 0;
			virtual bool Test(Stream* stream);
		};
	}
}