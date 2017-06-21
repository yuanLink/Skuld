#include "Common.h"

namespace Skuld
{
	namespace Algorithm
	{
		class CRC32 : public Hash
		{
		public:

			static bool mHasInit;
			static uint32_t mTable[256];

			uint32_t mHash = 0xffffffff;

			virtual ~CRC32() {}
			virtual size_t HashSize() const { return sizeof(uint32_t); }
			virtual void AppendData(const uint8_t* data, size_t size);
			virtual void GetHash(uint8_t* hash);
		};

		uint32_t CRC32::mTable[256] = { 0 };
		bool CRC32::mHasInit = false;

		void CRC32::AppendData(const uint8_t * data, size_t size)
		{
			if (!mHasInit)
			{
				for (uint32_t i = 0; i < 256; i++) 
				{
					uint32_t crc = i;
					for (uint32_t  j = 0; j < 8; j++) 
					{
						if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
						else crc >>= 1;
					}
					mTable[i] = crc;
				}
				mHasInit = true;
			}

			for (size_t i = 0; i < size; i++)
				mHash = (mHash >> 8) ^ mTable[(mHash & 0xFF) ^ data[i]];
		}

		void CRC32::GetHash(uint8_t * hash)
		{
			*(uint32_t*)(hash) = mHash ^ 0xffffffff;
		}

		class CRC16 : public Hash
		{
		public:

			static bool mHasInit;
			static uint16_t mTable[256];

			uint16_t mHash = 0;

			virtual ~CRC16() {}
			virtual size_t HashSize() const { return sizeof(uint16_t); }
			virtual void AppendData(const uint8_t* data, size_t size);
			virtual void GetHash(uint8_t* hash);
		};

		uint16_t CRC16::mTable[256] = { 0 };
		bool CRC16::mHasInit = false;
		
		void CRC16::AppendData(const uint8_t * data, size_t size)
		{
			if (!mHasInit)
			{
				for (uint16_t i = 0; i < 256; i++)
				{
					uint16_t crc = i;
					for (uint16_t j = 0; j < 8; j++)
					{
						if (crc & 0x1) crc = (crc >> 1) ^ 0xA001;
						else crc >>= 1;
					}
					mTable[i] = crc;
				}
				mHasInit = true;
			}
			for (size_t i = 0; i < size; i++)
				mHash = (mHash >> 8) ^ mTable[(mHash & 0xFF) ^ data[i]];
		}

		void CRC16::GetHash(uint8_t * hash)
		{
			*(uint16_t*)(hash) = mHash;
		}

		Hash* Hash_CreateCRC32() { return new CRC32(); }
		Hash* Hash_CreateCRC16() { return new CRC16(); }
	}
}