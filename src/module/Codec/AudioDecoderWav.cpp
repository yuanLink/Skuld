#include "AudioDecoder.h"
#include <Skuld/Ptr.hpp>
#include "../Support/MathLib.h"

namespace Skuld
{
	namespace Codec
	{
		class AudioDecoderWav : public AudioDecoder
		{
			virtual bool Test(Stream* stream);
			virtual AudioDecodeStream* Open(Stream* mStream);
		};

		std::shared_ptr<AudioDecoder> CreateAudioDecoderWav() {
			return std::make_shared<AudioDecoderWav>();
		}

		bool AudioDecoderWav::Test(Stream * stream)
		{
			char flag_buf[4];
			if (stream->Peek(flag_buf, 4) < 4) return false;
			if (memcmp(flag_buf, "RIFF", 4) != 0) return false;
			return true;
		}

		class WavAudioDecodeStream : public AudioDecodeStream
		{
		protected:
			virtual ~WavAudioDecodeStream() {}
		public:
			size_t mBegin;
			size_t mLength;
			Ptr<Stream> mStream;
			WaveInfo mInfo;

			WavAudioDecodeStream(Stream* mStream) : mStream(mStream) {
				mStream->AddRef();
			}

			virtual size_t Read(void* mBuffer, size_t mSize);
			virtual void SetPosition(size_t mPosition) { 
				mStream->Seek(mBegin + mPosition); 
			}
			virtual size_t GetPosition() const { 
				return mStream->GetPosition() - mBegin;
			}
			virtual size_t GetLength() const { return mLength; }
			virtual const WaveInfo& GetWaveInfomation() const { return mInfo; }
		};

		AudioDecodeStream * AudioDecoderWav::Open(Stream * mStream)
		{
			char flag_buf[4];
			mStream->Read(flag_buf, 4);

			if (memcmp(flag_buf, "RIFF", 4) != 0) return nullptr;

			uint32_t wav_size;
			mStream->Read(&wav_size);

			mStream->Read(flag_buf, 4);
			if (memcmp(flag_buf, "WAVE", 4) != 0) return nullptr;

			mStream->Read(flag_buf, 4);
			if (memcmp(flag_buf, "fmt ", 4) != 0) return nullptr;

			Ptr<WavAudioDecodeStream> mRet(new WavAudioDecodeStream(mStream));

			uint32_t st;
			mStream->Read(&st);
			mStream->Read(&mRet->mInfo, st);
			mRet->mInfo.cbSize = sizeof(mRet->mInfo);

			mStream->Read(flag_buf, 4);
			if (memcmp(flag_buf, "data", 4) != 0) return nullptr;

			uint32_t buflen;
			mStream->Read(&buflen);

			mRet->mLength = static_cast<size_t>(buflen);
			mRet->mBegin = mStream->GetPosition();
			return mRet.Detach();
		}

		size_t WavAudioDecodeStream::Read(void * mBuffer, size_t mSize)
		{
			return mStream->Read(mBuffer, 
				Min(mSize, mLength - (mStream->GetPosition() - mBegin))
			);
		}
	}
}