#include "Common.h"
#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Algorithm 
	{
		size_t CompressedStream::Peek(void * buffer, size_t size)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		int32_t CompressedStream::Seek(size_t position)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		int32_t CompressedStream::Offset(long offset)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t CompressedStream::GetPosition() const
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t CompressedStream::GetLength() const
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t EncryptedStream::Peek(void * buffer, size_t size)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		int32_t EncryptedStream::Seek(size_t position)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		int32_t EncryptedStream::Offset(long offset)
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t EncryptedStream::GetPosition() const
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t EncryptedStream::GetLength() const
		{
			throw Exception("不支持的方法");
			return 0;
		}

		size_t Hash_GetHashSize(Hash* hash) { return hash->HashSize(); }
		void Hash_AppendData(Hash* hash, const uint8_t* data, size_t size) { hash->AppendData(data, size); }
		void Hash_GetHash(Hash* hash, uint8_t* data) { hash->GetHash(data); }
		void Hash_Destroy(Hash* hash) { delete hash; }
	}
}