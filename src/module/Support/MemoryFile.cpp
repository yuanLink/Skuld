#include <string.h>
#include <new>
#include <Skuld/MemoryFile.h>

namespace Skuld
{
	struct MemoryFile
	{
		uint8_t* buf;
		size_t current;
		size_t size;
	};

	size_t memread(void * buffer, size_t size, size_t count, MEMFILE* _str)
	{
		MemoryFile* str = (MemoryFile*)_str;
		if (str == nullptr) return ((size_t)-1);
		size_t l = size*count;
		if (l + str->current > str->size)l = str->size - str->current;
		uint8_t * b = str->buf + str->current;
		memcpy(buffer, b, l);
		str->current += l;
		return l / size;
	}

	long memtell(MEMFILE * _str)
	{
		MemoryFile* str = (MemoryFile*)_str;
		if (str == nullptr) return -1;
		return (long)str->current;
	}

	int32_t memseek(MEMFILE * _str, long long offset, int32_t type)
	{
		MemoryFile* str = (MemoryFile*)_str;
		if (str == nullptr) return -1;
		switch (type)
		{
		case 0:
			if ((long long)str->size < offset) str->current = str->size;
			else str->current = offset;
			break;
		case 1:
			if (str->size < (offset + str->current)) str->current = str->size;
			else str->current += offset;
			break;
		case 2:
			if ((long long)str->size < offset) str->current = 0;
			else str->current = str->size - offset;
			break;
		default:
			break;
		}
		return (int32_t)str->current;
	}

	int32_t memclose(MEMFILE * _str)
	{
		MemoryFile* str = (MemoryFile*)_str;
		delete str;
		return 0;
	}

	int32_t memclose_free_buffer(MEMFILE * _str)
	{
		MemoryFile* str = (MemoryFile*)_str;
		delete str->buf;
		delete str;
		return 0;
	}

	MEMFILE* memopen(uint8_t* buf, size_t size)
	{
		MemoryFile* m = new (std::nothrow) MemoryFile;
		m->buf = buf;
		m->current = 0;
		m->size = size;
		return m;
	}
}