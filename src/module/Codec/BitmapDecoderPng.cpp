#include "BitmapDecoder.h"
#include <memory>
#include <png.h>
#include <Skuld/Ptr.hpp>
#include <string.h>

extern "C" int png_image_begin_read_from_userdata(png_imagep image, void *data, png_rw_ptr read_fn);

namespace Skuld
{
	namespace Codec
	{
		class BitmapDecoderPng : public BitmapDecoder
		{
			virtual Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex);
			virtual bool Test(Stream* stream);
		};

		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderPng()
		{
			return std::make_shared<BitmapDecoderPng>();
		}

		static void png_stream_read(png_structp png, png_bytep buffer, png_size_t size)
		{ 
			((Stream*)png_get_io_ptr(png))->Read(buffer, static_cast<uint32_t>(size)); 
		}

		Bitmap * BitmapDecoderPng::Decode(Stream * stream, uint32_t mBitmapIndex)
		{
			png_image image;
			memset(&image, 0, sizeof(png_image));
			image.version = PNG_IMAGE_VERSION;

			png_image_begin_read_from_userdata(&image, stream, png_stream_read);
			if (!PNG_IMAGE_FAILED(image))
			{
				uint32_t h = image.height;
				uint32_t w = image.width;

				PixelFormat format = PixelFormat_RGBA_8888;
				if (image.format == PNG_FORMAT_BGRA) format = PixelFormat_BGRA_8888;
				else if (image.format == PNG_FORMAT_RGBA) format = PixelFormat_RGBA_8888;
				else if (image.format == PNG_FORMAT_RGB) format = PixelFormat_RGB_888;
				else if (image.format == PNG_FORMAT_BGR) format = PixelFormat_BGR_888;
				else image.format = PNG_FORMAT_RGBA;

				Ptr<Bitmap> ret = Bitmap::CreateBitmap(w, h, format);

				if (!png_image_finish_read(&image, NULL, ret->GetPixels(), 0, nullptr))
				{
					png_image_free(&image);
					return nullptr;
				}

				png_image_free(&image);
				return ret.Detach();
			}

			return nullptr;
		}

		bool BitmapDecoderPng::Test(Stream * stream)
		{
			const uint8_t png_flag[] = { 137, 80, 78, 71, 13, 10, 26, 10 };
			uint8_t flag[8];
			if (stream->Peek(flag, 8) == 8)
				return (memcmp(flag, png_flag, 8) == 0);
			else return false;
		}
	}
}