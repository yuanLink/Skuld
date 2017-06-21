#include "BitmapEncoder.h"
#include "BitmapDecoder.h"
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoderTga : public BitmapEncoder
		{
		public:
			virtual BitmapFormat Format() { return BitmapFormat_TGA; }
			virtual void Encode(const Bitmap* bmp, Stream* stream);
		};

		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderTga() {
			return std::make_shared<BitmapEncoderTga>();
		}

		void BitmapEncoderTga::Encode(const Bitmap* bmp, Stream* mStream)
		{
			tga_header header;
			memset(&header, 0, sizeof(tga_header));

			header.tCompression = 2;
			header.tImageParams.iHeight = static_cast<int16_t>(bmp->GetHeight());
			header.tImageParams.iWidth = static_cast<int16_t>(bmp->GetWidth());
			PixelFormat mPixelFormat = bmp->GetPixelFormat();
			switch (mPixelFormat)
			{
			case PixelFormat_BGRA_8888:
			case PixelFormat_RGBA_8888:
				header.tImageParams.iBits = 32;
				header.tImageParams.iImageDescribedByte = 8;
				break;
			case PixelFormat_BGR_888:
			case PixelFormat_RGB_888:
				header.tImageParams.iBits = 24;
				header.tImageParams.iImageDescribedByte = 0;
				break;
			case PixelFormat_RGB_565:
			case PixelFormat_ARGB_1555:
				header.tImageParams.iBits = 16;
				header.tImageParams.iImageDescribedByte = 1;
				break;
			default:
				break;
			}
			mStream->Write(&header, sizeof(header));
			Ptr<Bitmap> mTemp;
			const uint8_t* mBuffer;
			uint32_t mStride = 0;

			if (mPixelFormat == PixelFormat_RGBA_8888)
			{
				mTemp.Detach(bmp->Convert(PixelFormat_BGRA_8888));
				mBuffer = mTemp->GetPixels();
				mStride = bmp->GetWidth() * 4;
			}
			else if (mPixelFormat == PixelFormat_BGRA_8888)
			{
				mBuffer = bmp->GetPixels();
				mStride = bmp->GetWidth() * 4;
			}
			else if (mPixelFormat == PixelFormat_RGB_888)
			{
				mTemp.Detach(bmp->Convert(PixelFormat_BGR_888));
				mBuffer = mTemp->GetPixels();
				mStride = bmp->GetWidth() * 3;
			}
			else if (mPixelFormat == PixelFormat_BGR_888)
			{
				mBuffer = bmp->GetPixels();
				mStride = bmp->GetWidth() * 3;
			}
			else if (mPixelFormat == PixelFormat_RGB_565)
			{
				mTemp.Detach(bmp->Convert(PixelFormat_ARGB_1555));
				mBuffer = mTemp->GetPixels();
				mStride = bmp->GetWidth() * 2;
			}
			else if (mPixelFormat == PixelFormat_ARGB_1555)
			{
				mBuffer = bmp->GetPixels();
				mStride = bmp->GetWidth() * 2;
			}

			for (int16_t i = 0; i < header.tImageParams.iHeight; i++)
				mStream->Write(&mBuffer[mStride * (header.tImageParams.iHeight - 1 - i)], mStride);
		}
	}
}