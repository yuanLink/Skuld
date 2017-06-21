#pragma once

#include "../Stream.h"

namespace Skuld
{
	namespace Algorithm
	{
		enum CompressionAlgorithm
		{
			CompressionAlgorithm_Deflate,
			CompressionAlgorithm_LZMA
		};

		enum CompressionMode
		{
			CompressionMode_Compress,
			CompressionMode_Decompress
		};

		class CompressedStream : public Stream
		{
		public:
			virtual CompressionAlgorithm GetCompressType() const = 0;
			virtual Stream* GetBindStream() const = 0;
			virtual size_t Peek(void* buffer, size_t size);
			virtual int32_t Seek(size_t position);
			virtual int32_t Offset(long offset);
			virtual size_t GetPosition() const;
			virtual size_t GetLength() const;
#if defined(SKULD_USE_DYNAMIC) || defined(SKULD_BUILD_DYNAMIC)
			static SKULD_EXPORT CompressedStream* CreateCompressedStreamFromStream(CompressionAlgorithm algorithm,
				Stream* stream, CompressionMode mode);
#endif

			static SKULD_EXPORT CompressedStream* CreateDeflateStream(Stream* stream, CompressionMode mode);
			static SKULD_EXPORT CompressedStream* CreateLZMAStream(Stream* stream, CompressionMode mode);
		};
	}
}