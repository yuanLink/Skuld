#include "BitmapEncoder.h"
#include <png.h>
#include <pngstruct.h>
#include <Skuld/Ptr.hpp>

extern "C" int png_image_write_to_userdata(png_imagep image, void* userdata, int convert_to_8bit,
	const void *buffer, png_int_32 row_stride, const void *colormap, png_rw_ptr write_fn, png_flush_ptr flush_fn);

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoderPng : public BitmapEncoder
		{
		public:
			virtual BitmapFormat Format() { return BitmapFormat_PNG; }
			virtual void Encode(const Bitmap* bmp, Stream* stream);
		};

		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderPng()
		{
			return std::make_shared<BitmapEncoderPng>();
		}

		static void png_stream_write(png_structp png, png_bytep buffer, png_size_t size) {
			reinterpret_cast<Stream*>(png->io_ptr)->Write(buffer, size);
		}

		static void png_stream_flush(png_structp png) {
			reinterpret_cast<Stream*>(png->io_ptr)->Flush();
		}

		void BitmapEncoderPng::Encode(const Bitmap * bmp, Stream * stream)
		{
			png_image img;
			memset(&img, 0, sizeof(png_image));
			img.version = PNG_IMAGE_VERSION;

			img.width = bmp->GetWidth();
			img.height = bmp->GetHeight();

			PixelFormat mPixelFormat = bmp->GetPixelFormat();
			if (mPixelFormat == PixelFormat_RGB_565)
			{
				Ptr<Bitmap> mCvt(bmp->Convert(PixelFormat_RGB_888));
				img.format = PNG_FORMAT_RGB;
				png_image_write_to_userdata(&img, stream, 0, mCvt->GetPixels(), 0,
					NULL, png_stream_write, png_stream_flush);
			}
			else if (mPixelFormat == PixelFormat_ARGB_1555)
			{
				Ptr<Bitmap> mCvt(bmp->Convert(PixelFormat_RGBA_8888));
				img.format = PNG_FORMAT_RGBA;
				png_image_write_to_userdata(&img, stream, 0, mCvt->GetPixels(), 0,
					NULL, png_stream_write, png_stream_flush);
			}
			else
			{
				switch (mPixelFormat)
				{
				case PixelFormat_RGBA_8888:
					img.format = PNG_FORMAT_RGBA;
					break;
				case PixelFormat_BGRA_8888:
					img.format = PNG_FORMAT_BGRA;
					break;
				case PixelFormat_RGB_888:
					img.format = PNG_FORMAT_RGB;
					break;
				case PixelFormat_BGR_888:
					img.format = PNG_FORMAT_BGR;
					break;
				default:
					break;
				}
				png_image_write_to_userdata(&img, stream, 0, bmp->GetPixels(), 0,
					NULL, png_stream_write, png_stream_flush);
			}
		}
	}
}