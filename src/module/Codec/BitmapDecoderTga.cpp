#include "BitmapDecoder.h"
#include <Skuld/Ptr.hpp>
#include <memory>

namespace Skuld
{
	namespace Codec
	{
		class BitmapDecoderTga : public BitmapDecoder
		{
			virtual Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex);
		};

		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderTga() {
			return std::make_shared<BitmapDecoderTga>();
		}

		Bitmap * BitmapDecoderTga::Decode(Stream * stream, uint32_t mBitmapIndex)
		{
			tga_header header;
			if (stream->Read(&header, sizeof(header)) < sizeof(header)) return nullptr;

			if (header.tIsColorTableExist) return nullptr;

			if (header.tImageParams.iBits != 16 &&
				header.tImageParams.iBits != 24 && 
				header.tImageParams.iBits != 32) return nullptr;

			Ptr<Bitmap> ret = nullptr;
			stream->Offset(header.tImageInfoLength);
			if (header.tCompression == TGA_NO_COMPRESS)
			{
				PixelFormat mPixelFormat = PixelFormat_BGRA_8888;
				if (header.tImageParams.iBits == 16) mPixelFormat = PixelFormat_ARGB_1555;
				else if (header.tImageParams.iBits == 24) mPixelFormat = PixelFormat_BGR_888;

				ret.Detach(Bitmap::CreateBitmap(
					static_cast<uint32_t>(header.tImageParams.iWidth),
					static_cast<uint32_t>(header.tImageParams.iHeight),
					mPixelFormat
				));
				
				auto stride = header.tImageParams.iWidth * (header.tImageParams.iBits >> 3);
				for (int16_t i = 0; i < header.tImageParams.iHeight; i++)
					if (stream->Read(&ret->GetPixels()[stride * (header.tImageParams.iHeight - 1 - i)], stride) != stride)
						return nullptr;
			}
			else if (header.tCompression == TGA_RLE)
			{
				PixelFormat mPixelFormat = PixelFormat_BGRA_8888;
				if (header.tImageParams.iBits == 16) mPixelFormat = PixelFormat_ARGB_1555;
				else if (header.tImageParams.iBits == 24) mPixelFormat = PixelFormat_BGR_888;

				ret.Detach(Bitmap::CreateBitmap(
					static_cast<uint32_t>(header.tImageParams.iWidth),
					static_cast<uint32_t>(header.tImageParams.iHeight),
					mPixelFormat
				));

				uint8_t* buffer = ret->GetPixels();

				uint32_t x = 0, y = header.tImageParams.iHeight - 1;
				uint32_t bytePerPixel = header.tImageParams.iBits >> 3;

				uint32_t total_pixel = 0;
				do
				{
					uint8_t flag;
					if (stream->Read(&flag, 1))
					{
						if (flag & 0x80)
						{
							flag = (flag & 0x7F) + 1;
							total_pixel += flag;
							uint8_t buf[4];
							if (stream->Read(buf, bytePerPixel) == bytePerPixel)
							{
								for (uint8_t i = 0; i < flag; i++)
								{
									uint32_t index = ((y * header.tImageParams.iHeight + x) * ((header.tImageParams.iBits < 24) ? 2 : 4));
									switch (bytePerPixel)
									{
									case 2: {
										uint16_t temp = buf[0] | (buf[1] << 8);
										buffer[index++] = temp & 0xFF;
										buffer[index++] = temp >> 8;
										break;
									}
									case 3:
										buffer[index++] = buf[0];
										buffer[index++] = buf[1];
										buffer[index++] = buf[2];
										buffer[index++] = 0xff;
										break;
									case 4:
										buffer[index++] = buf[0];
										buffer[index++] = buf[1];
										buffer[index++] = buf[2];
										buffer[index++] = buf[3];
										break;
									default:
										break;
									}
									x++;
									if (x == header.tImageParams.iWidth)
									{
										x = 0;
										y--;
									}
								}
							}
							else return nullptr;
						}
						else
						{
							flag = (flag & 0x7F) + 1;
							total_pixel += flag;
							uint8_t buf[4];
							for (uint8_t i = 0; i < flag; i++)
							{
								if (stream->Read(buf, bytePerPixel) == bytePerPixel)
								{
									uint32_t index = ((y * header.tImageParams.iHeight + x) * ((header.tImageParams.iBits < 24) ? 2 : 4));
									switch (bytePerPixel)
									{
									case 2: {
										uint16_t temp = buf[0] | (buf[1] << 8);
										buffer[index++] = temp & 0xFF;
										buffer[index++] = temp >> 8;
										break;
									}
									case 3:
										buffer[index++] = buf[0];
										buffer[index++] = buf[1];
										buffer[index++] = buf[2];
										break;
									case 4:
										buffer[index++] = buf[0];
										buffer[index++] = buf[1];
										buffer[index++] = buf[2];
										buffer[index++] = buf[3];
										break;
									default:
										break;
									}
									x++;
									if (x == header.tImageParams.iWidth)
									{
										x = 0;
										y--;
									}
								}
								else return nullptr;
							}
						}
					}
					else return nullptr;
				} while ((int32_t)total_pixel < header.tImageParams.iHeight * header.tImageParams.iWidth);
			}
			else return nullptr;

			return ret.Detach();
		}
	}
}