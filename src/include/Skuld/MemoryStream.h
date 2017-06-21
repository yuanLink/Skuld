#pragma once
#include "Stream.h"

namespace Skuld
{
	class SKULD_EXPORT MemoryStream : public Stream
	{
	protected:
		virtual ~MemoryStream();
	private:
		MemoryStream();
		MemoryStream(const MemoryStream&&);
		MemoryStream& operator=(const MemoryStream&);
		uint8_t* mem;
		size_t size;
		size_t real_size;
		size_t current;
		bool delete_mem;
		bool can_write;
	public:
		size_t GetTotalSpace();
		const void* GetMemoryAddress() const;
		static MemoryStream* Open(uint8_t* buffer, size_t size, bool delete_when_close = true);
		static MemoryStream* Open(size_t size);
		size_t Read(void* buffer, size_t size);
		size_t Write(const void* buffer, size_t size);
		int32_t Seek(size_t position);
		int32_t Offset(long offset);
		size_t GetPosition() const;
		bool CanRead() const;
		bool CanWrite() const;
		size_t GetLength() const;
		void Flush();
		bool EndOfStream() const;
		size_t Peek(void* buffer, size_t size);
	};
}