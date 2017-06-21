#pragma once
#include <map>
#include <functional>
#include <Skuld/Algorithm/CompressedStream.h>
#include <Skuld/Algorithm/EncryptedStream.h>
#include <Skuld/Algorithm/Hash.h>
#include "../Support/Factory.h"

namespace Skuld
{
	namespace Algorithm
	{
		typedef Factory<CompressedStream, CompressionAlgorithm,
			CompressedStream*(Stream*, CompressionMode)> CompressedStreamFactory;

		typedef Factory<EncryptedStream, EncryptionAlgorithm,
			EncryptedStream*(Stream*, EncryptionMode, const uint8_t*, size_t)> EncryptedStreamFactory;

		class Hash
		{
		public:
			virtual ~Hash() {}
			virtual size_t HashSize() const = 0;
			virtual void AppendData(const uint8_t* data, size_t size) = 0;
			virtual void GetHash(uint8_t* hash) = 0;
		};
	}
}