#pragma once
#include "../Export.h"
#include <stdint.h>

namespace Skuld
{
	namespace Algorithm
	{
		class Hash;

		SKULD_EXPORT size_t Hash_GetHashSize(Hash* hash);
		SKULD_EXPORT void Hash_AppendData(Hash* hash, const uint8_t* data, size_t size);
		SKULD_EXPORT void Hash_GetHash(Hash* hash, uint8_t* data);
		SKULD_EXPORT void Hash_Destroy(Hash* hash);

		SKULD_EXPORT Hash* Hash_CreateCRC32();
		SKULD_EXPORT Hash* Hash_CreateCRC16();
	}
}