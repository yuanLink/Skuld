#include "Common.h"
#include <zlib.h>
#include <Skuld/Exception.h>
#include "../Support/MathLib.h"

namespace Skuld
{
	namespace Algorithm
	{
		class DeflateEncStream : public CompressedStream
		{
		protected:
			virtual ~DeflateEncStream();

			Stream* mBase;
			z_stream m_z;
		public:
			DeflateEncStream(Stream* mBase) : 
				mBase(mBase)
			{
				memset(&m_z, 0, sizeof(z_stream));
				deflateInit(&m_z, Z_BEST_COMPRESSION);
			}

			virtual size_t Read(void* buffer, size_t size) { throw Exception("不能从流读取"); }
			virtual size_t Write(const void* buffer, size_t size);
			virtual CompressionAlgorithm GetCompressType() const { return CompressionAlgorithm_Deflate; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() {};
			virtual bool CanRead() const { return false; }
			virtual bool CanWrite() const { return true; }

			virtual bool EndOfStream() const
			{
				if (!CanRead()) throw Exception("不能确定流的末尾");

				return false;
			}
		};

		class DeflateDecStream : public CompressedStream
		{
		protected:
			virtual ~DeflateDecStream();

			Stream* mBase;
			z_stream m_z;

			uint8_t mCacheBuffer[8192];
			bool eof = false;
		public:
			DeflateDecStream(Stream* mBase) :
				mBase(mBase)
			{
				memset(&m_z, 0, sizeof(z_stream));
				inflateInit(&m_z);
			}

			virtual size_t Read(void* buffer, size_t size);
			virtual size_t Write(const void* buffer, size_t size) { throw Exception("不能写入流");  return 0; }
			virtual CompressionAlgorithm GetCompressType() const { return CompressionAlgorithm_Deflate; }
			virtual Stream* GetBindStream() const { return mBase; }
			virtual void Flush() {};
			virtual bool CanRead() const { return true; }
			virtual bool CanWrite() const { return false; }

			virtual bool EndOfStream() const { return eof; }
		};

		CompressedStream* CompressedStream::CreateDeflateStream(Stream* stream, CompressionMode mode)
		{
			if (mode == CompressionMode_Compress)
			{
				stream->AddRef();
				return new DeflateEncStream(stream);
			}
			else if (mode == CompressionMode_Decompress)
			{
				stream->AddRef();
				return new DeflateDecStream(stream);
			}
			return nullptr;
		}

		DeflateEncStream::~DeflateEncStream()
		{
			Bytef mCacheBuffer[8192];
			do
			{
				m_z.next_out = mCacheBuffer;
				m_z.avail_out = sizeof(mCacheBuffer);

				deflate(&m_z, Z_FINISH);

				mBase->Write(mCacheBuffer, sizeof(mCacheBuffer) - m_z.avail_out);
			} while (m_z.avail_in != 0);
			deflateEnd(&m_z);
			Flush();
			mBase->Dispose();
		}

		DeflateDecStream::~DeflateDecStream()
		{
			inflateEnd(&m_z);
			mBase->Dispose();
		}

		size_t DeflateDecStream::Read(void * buffer, size_t size)
		{
			if (!CanRead()) throw Exception("不能从流读取");

			m_z.next_out = (Bytef*)buffer;
			m_z.avail_out = (uInt)size;

			while (true)
			{
				if (m_z.avail_in == 0) {
					size_t in = mBase->Read(mCacheBuffer, sizeof(mCacheBuffer));
					m_z.avail_in = (uInt)in;
					m_z.next_in = mCacheBuffer;
				}
				int err = inflate(&m_z, Z_NO_FLUSH);

				if (err == Z_STREAM_END) eof = true;
				if (m_z.avail_out == 0 || err == Z_STREAM_END) break;
			}

			return size - (size_t)m_z.avail_out;
		}

		size_t DeflateEncStream::Write(const void * buffer, size_t size)
		{
			if (!CanWrite()) throw Exception("不能写入流");

			m_z.next_in = (Bytef*)(buffer);
			m_z.avail_in = (uInt)size;

			Bytef mCacheBuffer[8192];

			do
			{
				m_z.next_out = mCacheBuffer;
				m_z.avail_out = sizeof(mCacheBuffer);

				deflate(&m_z, Z_NO_FLUSH);

				mBase->Write(mCacheBuffer, sizeof(mCacheBuffer) - m_z.avail_out);
			} while (m_z.avail_in != 0);

			return size;
		}
	}
}