#include "Common.h"
#include <vector>

namespace Skuld
{
	namespace Algorithm
	{
		class AESStream : public EncryptedStream
		{
		protected:
			virtual ~AESStream() { mBase->Dispose(); }

			Stream* mBase;
			EncryptionMode mMode;
			std::vector<uint8_t> mpassword;
		public:
			AESStream(Stream* mBase, EncryptionMode mMode, const uint8_t* pad, size_t pad_size) :
				mBase(mBase), mMode(mMode) {

			}
			virtual size_t Read(void* buffer, size_t size);
			virtual size_t Write(const void* buffer, size_t size);
			virtual EncryptionAlgorithm GetEncryptedType() const { return EncryptionAlgorithm_AES; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() { mBase->Flush(); }
			virtual bool CanRead() const { return mMode == EncryptionMode_Encrypt; }
			virtual bool CanWrite() const { return mMode == EncryptionMode_Decrypt; }

			virtual bool EndOfStream() const { return mBase->EndOfStream(); }
		};

		EncryptedStream* EncryptedStream::CreateAESStream(Stream* mBase, EncryptionMode mMode, const uint8_t* password, size_t password_size)
		{
			mBase->AddRef();
			return new AESStream(mBase, mMode, password, password_size);
		}

		static EncryptedStreamFactory::Register mReg(EncryptedStream::CreateAESStream, EncryptionAlgorithm_AES);

		size_t AESStream::Read(void * buffer, size_t size)
		{
			return size_t();
		}

		size_t AESStream::Write(const void * buffer, size_t size)
		{
			return size_t();
		}
	}
}