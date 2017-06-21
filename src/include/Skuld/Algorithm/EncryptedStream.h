#pragma once

#include "../Stream.h"

namespace Skuld
{
	namespace Algorithm
	{
		enum EncryptionAlgorithm
		{
			EncryptionAlgorithm_XOR,
			EncryptionAlgorithm_AES
		};

		enum EncryptionMode
		{
			EncryptionMode_Encrypt,
			EncryptionMode_Decrypt
		};

		class EncryptedStream : public Stream
		{
		public:
			virtual EncryptionAlgorithm GetEncryptedType() const = 0;
			virtual Stream* GetBindStream() const = 0;
			virtual size_t Peek(void* buffer, size_t size);
			virtual int32_t Seek(size_t position);
			virtual int32_t Offset(long offset);
			virtual size_t GetPosition() const;
			virtual size_t GetLength() const;
#if defined(SKULD_USE_DYNAMIC) || defined(SKULD_BUILD_DYNAMIC)
			static SKULD_EXPORT EncryptedStream* CreateEncryptedStreamFromStream(EncryptionAlgorithm algorithm,
				Stream* stream, EncryptionMode mode, const uint8_t* password, size_t password_size);
#endif
			static SKULD_EXPORT EncryptedStream* CreateXORStream(Stream* stream, EncryptionMode mode, const uint8_t* password, size_t password_size);
			static SKULD_EXPORT EncryptedStream* CreateAESStream(Stream* stream, EncryptionMode mode, const uint8_t* password, size_t password_size);
		};
	}
}