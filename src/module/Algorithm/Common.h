#pragma once
#include <map>
#include <functional>
#include <Skuld/Algorithm/CompressedStream.h>
#include <Skuld/Algorithm/EncryptedStream.h>
#include <Skuld/Algorithm/Hash.h>

namespace Skuld
{
	namespace Algorithm
	{
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