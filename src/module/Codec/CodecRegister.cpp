#include <Skuld/Codec/AudioCodec.h>
#include <Skuld/Codec/BitmapCodec.h>
#include <Skuld/Codec/VideoCodec.h>
#include <Skuld/Codec/TextureCodec.h>
#include <vector>
#include "BitmapDecoder.h"
#include "BitmapEncoder.h"
#include "AudioDecoder.h"
#include "AudioEncoder.h"
#include "VideoDecoder.h"
#include "VideoEncoder.h"
#include "TextureDecoder.h"
#include <memory>
#include <functional>

namespace Skuld
{
	namespace Codec
	{
		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderPng();
		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderEtc1();
		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderIcraw();
		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderTga();
		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderJpeg();
		std::shared_ptr<AudioDecoder> CreateAudioDecoderOgg();
		std::shared_ptr<AudioDecoder> CreateAudioDecoderWav();
		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderPng();
		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderIcraw();
		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderEtc1();
		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderTga();
		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderJpeg();

		class CodecRegister {
		public:
			template<typename T>
			class TypeRegister
			{
			public:
				std::vector<std::shared_ptr<T>> mObjects;
				void Register(std::function<std::shared_ptr<T>()> func) { mObjects.push_back(func()); }
			};

			TypeRegister<BitmapDecoder> mBitmapDecoders;
			TypeRegister<BitmapEncoder> mBitmapEncoders;
			TypeRegister<AudioDecoder> mAudioDecoders;
			TypeRegister<AudioEncoder> mAudioEncoders;
			TypeRegister<VideoDecoder> mVideoDecoders;
			TypeRegister<VideoEncoder> mVideoEncoders;
			TypeRegister<TextureDecoder> mTextureDecoders;

			CodecRegister() {
				mBitmapDecoders.Register(CreateBitmapDecoderPng);
				mBitmapDecoders.Register(CreateBitmapDecoderEtc1);
				mBitmapDecoders.Register(CreateBitmapDecoderIcraw);
				mBitmapDecoders.Register(CreateBitmapDecoderTga);
				mBitmapDecoders.Register(CreateBitmapDecoderJpeg);

				mBitmapEncoders.Register(CreateBitmapEncoderPng);
				mBitmapEncoders.Register(CreateBitmapEncoderIcraw);
				mBitmapEncoders.Register(CreateBitmapEncoderEtc1);
				mBitmapEncoders.Register(CreateBitmapEncoderTga);
				mBitmapEncoders.Register(CreateBitmapEncoderJpeg);

				mAudioDecoders.Register(CreateAudioDecoderOgg);
				mAudioDecoders.Register(CreateAudioDecoderWav);
			}
		};

		static CodecRegister mRegister;

		Bitmap* DecodeBitmapFromStream(Stream* stream, uint32_t mBitmapIndex)
		{
			for (std::shared_ptr<BitmapDecoder>& it : mRegister.mBitmapDecoders.mObjects)
			{
				Bitmap* ret = nullptr;
				if (it->Test(stream))
				{
					size_t mBegin = stream->GetPosition();
					ret = it->Decode(stream, mBitmapIndex);
					if (ret == nullptr)
						stream->Seek(mBegin);
				}
				if (ret) return ret;
			}
			return nullptr;
		}

		AudioDecodeStream* DecodeAudioFromStream(Stream* stream)
		{
			for (std::shared_ptr<AudioDecoder>& it : mRegister.mAudioDecoders.mObjects)
			{
				AudioDecodeStream* ret = nullptr;
				if (it->Test(stream))
				{
					size_t mBegin = stream->GetPosition();
					ret = it->Open(stream);
					if (ret == nullptr)
						stream->Seek(mBegin);
				}
				if (ret) return ret;
			}
			return nullptr;
		}

		void EncodeBitmapFromStream(Stream * stream, const Bitmap * bmp, BitmapFormat format)
		{
			for (std::shared_ptr<BitmapEncoder>& it : mRegister.mBitmapEncoders.mObjects)
				if (it->Format() == format)
					it->Encode(bmp, stream);
		}

		bool BitmapDecoder::Test(Stream * stream)
		{
			return true;
		}

		bool VideoDecoder::Test(Stream * stream)
		{
			return true;
		}

		bool AudioDecoder::Test(Stream * stream)
		{
			return true;
		}

		BitmapFormat BitmapFormatFromString(const String& mStr)
		{
			String str = mStr.ToLower();
			if (str == "png") return BitmapFormat_PNG;
			else if (str == "etc1") return BitmapFormat_ETC1;
			else if (str == "tga") return BitmapFormat_TGA;
			else if ((str == "jpg") || (str == "jpeg")) return BitmapFormat_JPEG;
			else return BitmapFormat_ICRAW;
		}
	}
}