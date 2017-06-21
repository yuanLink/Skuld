#pragma once
#include "Export.h"
#include <stddef.h>

namespace Skuld
{
	typedef void MEMFILE;

	SKULD_EXPORT MEMFILE* memopen(uint8_t* buf, size_t size);
	SKULD_EXPORT int32_t memclose(MEMFILE * _str);
	SKULD_EXPORT int32_t memclose_free_buffer(MEMFILE * _str);
	SKULD_EXPORT int32_t memseek(MEMFILE * _str, long long offset, int32_t type);
	SKULD_EXPORT long memtell(MEMFILE * _str);
	SKULD_EXPORT size_t memread(void * buffer, size_t size, size_t count, MEMFILE* _str);
}