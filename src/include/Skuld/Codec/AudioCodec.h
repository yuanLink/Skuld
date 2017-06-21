#pragma once

#include "../Stream.h"

namespace Skuld
{
	namespace Codec
	{
		struct WaveInfo
		{
			uint16_t wFormatTag;
			uint16_t nChannels;
			uint32_t nSamplesPerSec;
			uint32_t nAvgBytesPerSec;
			uint16_t nBlockAlign;
			uint16_t wBitsPerSample;
			uint16_t cbSize;
		};

		enum AudioFormat
		{
			AudioFormat_Ogg,
			AudioFormat_Wav,
		};

		class AudioDecodeStream : public IDisposable
		{
		protected:
			virtual ~AudioDecodeStream() {}
		public:
			virtual size_t Read(void* mBuffer, size_t mSize) = 0;
			virtual void SetPosition(size_t mPosition) = 0;
			virtual size_t GetPosition() const = 0;
			virtual size_t GetLength() const = 0;
			virtual const WaveInfo& GetWaveInfomation() const = 0;
		};

		class Audio;

		SKULD_EXPORT AudioDecodeStream* DecodeAudioFromStream(Stream* stream);
		SKULD_EXPORT void EncodeAudioFromStream(Stream* stream, Audio* audio, AudioFormat format);
	}
}