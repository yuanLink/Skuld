#include "AudioDecoder.h"
#include <Skuld/Ptr.hpp>
#include <vorbis/vorbisfile.h>

namespace Skuld
{
	namespace Codec
	{
		static size_t ov_read_stream(void* buf, size_t length, size_t count, void* handle) {
			return reinterpret_cast<Stream*>(handle)->Read(buf, length * count);
		}
		
		static int ov_seek_stream(void* handle, ogg_int64_t offset, int direct) {
			if (direct == 0)
				reinterpret_cast<Stream*>(handle)->Seek(static_cast<size_t>(offset));
			else if (direct == 1)
				return reinterpret_cast<Stream*>(handle)->Offset(static_cast<long>(offset));
			else if (direct == 2)
				return reinterpret_cast<Stream*>(handle)->Seek(
					static_cast<size_t>(reinterpret_cast<Stream*>(handle)->GetLength() - offset));
			return 0;
		}

		static long ov_tell_stream(void* handle) {
			return static_cast<long>(reinterpret_cast<Stream*>(handle)->GetPosition());
		}

		static const ov_callbacks OV_CALLBACKS_STREAM = {
			ov_read_stream,
			ov_seek_stream,
			nullptr,
			ov_tell_stream
		};

		class AudioDecoderOgg : public AudioDecoder
		{
		public:
			virtual bool Test(Stream* mStream);
			virtual AudioDecodeStream* Open(Stream* mStream);
		};

		class OggDecodeStream : public AudioDecodeStream
		{
		protected:
			virtual ~OggDecodeStream();
		public:
			OggVorbis_File mFile;
			WaveInfo mInfo;
			
			Ptr<Stream> mStream;

			OggDecodeStream(Stream* mStream) : mStream(mStream) {
				mStream->AddRef();
			}

			virtual size_t Read(void* mBuffer, size_t mSize);
			virtual void SetPosition(size_t mPosition) {
				ov_pcm_seek(&mFile, static_cast<ogg_int64_t>(mPosition));
			}
			virtual size_t GetPosition() const {
				return static_cast<size_t>(ov_pcm_tell(const_cast<OggVorbis_File*>(&this->mFile)));
			}
			virtual size_t GetLength() const {
				return static_cast<size_t>(ov_pcm_total(const_cast<OggVorbis_File*>(&this->mFile), -1));
			}
			virtual const WaveInfo& GetWaveInfomation() const { return mInfo; }
		};

		std::shared_ptr<AudioDecoder> CreateAudioDecoderOgg()
		{
			return std::make_shared<AudioDecoderOgg>();
		}

		bool AudioDecoderOgg::Test(Stream * mStream)
		{
			char flag[4];
			if (mStream->Peek(flag, 4) < 4) return false;
			return memcmp("OggS", flag, 4) == 0;
		}

		AudioDecodeStream* AudioDecoderOgg::Open(Stream * mStream)
		{
			Ptr<OggDecodeStream> mRet(new OggDecodeStream(mStream));
			ov_open_callbacks(mStream, &mRet->mFile, nullptr, 0, OV_CALLBACKS_STREAM);

			vorbis_info* vi = ov_info(&mRet->mFile, -1);
			if (!vi) return nullptr;

			memset(&mRet->mInfo, 0, sizeof(WaveInfo));
			mRet->mInfo.cbSize = sizeof(WaveInfo);
			mRet->mInfo.nChannels = static_cast<uint16_t>(vi->channels);
			mRet->mInfo.wBitsPerSample = 16;//static_cast<uint16_t>(vi->bitrate_nominal / vi->rate / wf.nChannels); 
											//static_cast<uint16_t>((vi->bitrate_nominal / vi->rate + 7) / 8) << 3;
			mRet->mInfo.nSamplesPerSec = vi->rate;
			mRet->mInfo.nAvgBytesPerSec = mRet->mInfo.nSamplesPerSec*mRet->mInfo.nChannels * 
				mRet->mInfo.wBitsPerSample / 8;
			mRet->mInfo.nBlockAlign = mRet->mInfo.wBitsPerSample / 8 * mRet->mInfo.nChannels;
			mRet->mInfo.wFormatTag = 1;
			return mRet.Detach();
		}

		OggDecodeStream::~OggDecodeStream()
		{
			ov_clear(&mFile);
		}

		size_t OggDecodeStream::Read(void * mBuffer, size_t mSize)
		{
			int current_section;
			size_t mTotal = 0;
			while (mTotal < mSize)
			{
				long mTemp = ov_read(&mFile, reinterpret_cast<char*>(mBuffer) + mTotal,
					static_cast<int>(mSize - mTotal), 0, 2, 1, &current_section);
				mTotal += static_cast<size_t>(mTemp);
				if (mTemp == 0) break;
			}
			return mTotal;
		}
	}
}