#include "Common.h"
#include <Skuld/Exception.h>
#define LZMA_API_STATIC
#include <lzma.h>

namespace Skuld
{
	namespace Algorithm
	{
		class LZMAEncStream : public CompressedStream
		{
		protected:
			virtual ~LZMAEncStream();

			Stream* mBase;
			lzma_stream m_z;
		public:
			LZMAEncStream(Stream* mBase)
			{
				this->mBase = mBase; 
				m_z = LZMA_STREAM_INIT;
				lzma_easy_encoder(&m_z, 6, LZMA_CHECK_CRC32);
			}

			virtual size_t Read(void* buffer, size_t size) { throw Exception("不能从流读取"); }
			virtual size_t Write(const void* buffer, size_t size);
			virtual CompressionAlgorithm GetCompressType() const { return CompressionAlgorithm_LZMA; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() { mBase->Flush(); }
			virtual bool CanRead() const { return false; }
			virtual bool CanWrite() const { return true; }

			virtual bool EndOfStream() const {
				throw Exception("不能确定流的末尾");
			}
		};

		class LZMADecStream : public CompressedStream
		{
		protected:
			virtual ~LZMADecStream();

			Stream* mBase;
			lzma_stream m_z;

			uint8_t mCacheBuffer[8192];
			bool eof = false;
		public:
			LZMADecStream(Stream* mBase) :
				mBase(mBase)
			{
				m_z = LZMA_STREAM_INIT;
				lzma_stream_decoder(&m_z, UINT64_MAX, LZMA_TELL_UNSUPPORTED_CHECK);
			}

			virtual size_t Read(void* buffer, size_t size);
			virtual size_t Write(const void* buffer, size_t size) { throw Exception("不能写入流");  return 0; }
			virtual CompressionAlgorithm GetCompressType() const { return CompressionAlgorithm_LZMA; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() {};
			virtual bool CanRead() const { return true; }
			virtual bool CanWrite() const { return false; }

			virtual bool EndOfStream() const { return eof; }
		};

		LZMAEncStream::~LZMAEncStream()
		{
			uint8_t mCacheBuffer[8192];
			
			do
			{
				m_z.next_out = mCacheBuffer;
				m_z.avail_out = sizeof(mCacheBuffer);
				lzma_ret ret = lzma_code(&m_z, lzma_action::LZMA_FINISH);

				mBase->Write(mCacheBuffer, sizeof(mCacheBuffer) - m_z.avail_out);
			} while (m_z.avail_in != 0);
			lzma_end(&m_z);
			Flush();
			mBase->Dispose();
		}

		size_t LZMAEncStream::Write(const void * buffer, size_t size)
		{
			if (!CanWrite()) throw Exception("不能写入流");

			this->m_z.next_in = (const uint8_t*)buffer;
			this->m_z.avail_in = size;

			uint8_t mCacheBuffer[8192];

			do
			{
				this->m_z.next_out = mCacheBuffer;
				this->m_z.avail_out = sizeof(mCacheBuffer);
				lzma_code(&this->m_z, lzma_action::LZMA_RUN);

				this->mBase->Write(mCacheBuffer, sizeof(mCacheBuffer) - this->m_z.avail_out);
			} while (this->m_z.avail_in != 0);

			return size;
		}

		LZMADecStream::~LZMADecStream()
		{
			lzma_end(&m_z);
			mBase->Dispose();
		}

		size_t LZMADecStream::Read(void * buffer, size_t size)
		{
			if (!CanRead()) throw Exception("不能从流读取");

			m_z.next_out = (uint8_t*)buffer;
			m_z.avail_out = size;

			while (true)
			{
				if (m_z.avail_in == 0) {
					size_t in = mBase->Read(mCacheBuffer, sizeof(mCacheBuffer));
					m_z.avail_in = in;
					m_z.next_in = mCacheBuffer;
				}
				lzma_ret err = lzma_code(&m_z, LZMA_RUN);

				if (err == LZMA_STREAM_END) eof = true;
				if (m_z.avail_out == 0 || err == LZMA_STREAM_END) break;
			}

			return size - (size_t)m_z.avail_out;
		}

		CompressedStream* CompressedStream::CreateLZMAStream(Stream* mBase, CompressionMode mMode)
		{
			if (mMode == CompressionMode_Compress)
			{
				mBase->AddRef();
				return new LZMAEncStream(mBase);
			}
			else if (mMode == CompressionMode_Decompress)
			{
				mBase->AddRef();
				return new LZMADecStream(mBase);
			}
			else return nullptr;
		}
	}
}