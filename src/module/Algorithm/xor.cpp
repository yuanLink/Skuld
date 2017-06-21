#include "Common.h"
#include <vector>
#include <memory>

namespace Skuld
{
	namespace Algorithm
	{
		class XORStream : public EncryptedStream
		{
		protected:
			virtual ~XORStream() { mBase->Dispose(); }

			Stream* mBase;
			EncryptionMode mMode;
			std::vector<uint8_t> mPad;
			size_t mPadCurrent = 0;
		public:
			XORStream(Stream* mBase, EncryptionMode mMode, const uint8_t* pad, size_t pad_size) :
					mBase(mBase), mMode(mMode), mPad(pad_size) {
				memcpy(mPad.data(), pad, pad_size);
			}
			virtual size_t Read(void* buffer, size_t size);
			virtual size_t Write(const void* buffer, size_t size);
			virtual EncryptionAlgorithm GetEncryptedType() const { return EncryptionAlgorithm_XOR; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() { mBase->Flush(); }
			virtual bool CanRead() const { return mMode == EncryptionMode_Encrypt; }
			virtual bool CanWrite() const { return mMode == EncryptionMode_Decrypt; }

			virtual bool EndOfStream() const { return mBase->EndOfStream(); }
		};

		EncryptedStream * EncryptedStream::CreateXORStream(Stream * mBase, EncryptionMode mMode, const uint8_t * password, size_t password_size)
		{
			mBase->AddRef();
			return new XORStream(mBase, mMode, password, password_size);
		}

		static EncryptedStreamFactory::Register mReg(EncryptedStream::CreateXORStream, EncryptionAlgorithm_XOR);

		size_t XORStream::Read(void * buffer, size_t size)
		{
			uint8_t* buf = (uint8_t*)buffer;
			size_t ret = mBase->Read(buf, size);

			for (size_t i = 0; i < ret; i++)
			{
				buf[i] ^= mPad[mPadCurrent++];
				if (mPadCurrent >= mPad.size()) mPadCurrent = 0;
			}
			return ret;
		}

		size_t XORStream::Write(const void * buffer, size_t size)
		{
			uint8_t* buf2 = (uint8_t*)buffer;
			std::unique_ptr<uint8_t[]> buf = std::make_unique<uint8_t[]>(size);

			for (size_t i = 0; i < size; i++)
			{
				buf[i] = buf2[i] ^ mPad[mPadCurrent++];
				if (mPadCurrent >= mPad.size()) mPadCurrent = 0;
			}

			return mBase->Write(buf.get(), size);
		}
	}
}