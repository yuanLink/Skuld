#include <Skuld/MemoryStream.h>
#include <string.h>

namespace Skuld
{
	MemoryStream::MemoryStream() {}

	const void* MemoryStream::GetMemoryAddress() const
	{
		return this->mem;
	}

	size_t MemoryStream::GetTotalSpace()
	{
		return this->size;
	}

	MemoryStream* MemoryStream::Open(uint8_t* buffer, size_t size, bool delete_when_close /* = true */)
	{
		MemoryStream* ret = new (std::nothrow) MemoryStream;
		ret->can_write = delete_when_close;
		ret->delete_mem = delete_when_close;
		ret->mem = buffer;
		ret->size = size;
		ret->current = 0;
		ret->real_size = size;
		return ret;
	}

	MemoryStream* MemoryStream::Open(size_t size)
	{
		MemoryStream* ret = MemoryStream::Open(new (std::nothrow) uint8_t[size], size, true);
		ret->real_size = 0;
		return ret;
	}

	bool MemoryStream::CanWrite() const
	{
		return this->can_write;
	}

	bool MemoryStream::CanRead() const
	{
		return true;
	}

	size_t MemoryStream::Read(void* buffer, size_t _size)
	{
		size_t real_read_size = (_size + this->current <= this->size) ? _size : (this->size - this->current);

		memcpy(buffer, &this->mem[this->current], real_read_size);
		this->current += real_read_size;

		return real_read_size;
	}

	MemoryStream::~MemoryStream()
	{
		if (this->delete_mem) delete[] mem;
	}

	size_t MemoryStream::GetLength() const
	{
		return this->real_size;
	}

	size_t MemoryStream::GetPosition() const
	{
		return this->current;
	}

	void MemoryStream::Flush() {}

	bool MemoryStream::EndOfStream() const
	{
		return GetLength() <= GetPosition();
	}

	size_t MemoryStream::Peek(void* buffer, size_t _size)
	{
		size_t real_read_size = (_size + this->current <= this->size) ? _size : (this->size - this->current);

		memcpy(buffer, &this->mem[this->current], real_read_size);

		return real_read_size;
	}

	int32_t MemoryStream::Offset(long offset)
	{
		if (offset < 0 && offset < this->current) return -1;
		else
		{
			while (this->current + offset > this->size)
			{
				if (this->can_write == false)
				{
					return -1;
				}
				else
				{
					uint8_t* new_mem = new (std::nothrow) uint8_t[this->size * 2];
					memcpy(new_mem, this->mem, this->size);
					this->size *= 2;
					delete[] this->mem;
					this->mem = new_mem;
				}
			}
			this->current += offset;
			if (this->current > this->real_size) this->real_size = this->current;
			return 0;
		}
	}

	int32_t MemoryStream::Seek(size_t position)
	{
		while (position > this->size)
		{
			if (this->can_write == false)
			{
				return 0;
			}
			else
			{
				uint8_t* new_mem = new (std::nothrow) uint8_t[this->size * 2];
				memcpy(new_mem, this->mem, this->size);
				this->size *= 2;
				delete[] this->mem;
				this->mem = new_mem;
			}
		}
		this->current = position;
		if (this->current > this->real_size) this->real_size = this->current;
		return static_cast<int32_t>(position);
	}

	size_t MemoryStream::Write(const void* buffer, size_t _size)
	{
		if (this->can_write == false) return (size_t)~0;


		while (this->current + _size > this->size)
		{
			uint8_t* new_mem = new (std::nothrow) uint8_t[this->size * 2];
			memcpy(new_mem, this->mem, this->size);
			this->size *= 2;
			delete[] this->mem;
			this->mem = new_mem;
		}

		memcpy(&this->mem[this->current], buffer, _size);
		this->current += _size;
		if (this->current > this->real_size) this->real_size = this->current;
		return size;
	}
}