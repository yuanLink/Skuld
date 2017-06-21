#pragma once

#include "../Check.h"
#include "../Stream.h"

namespace Skuld
{
	namespace Storage
	{
		enum ResourceType
		{
			ResourceType_Video,
			ResourceType_Audio,
			ResourceType_Effect,
			ResourceType_Animation,
			ResourceType_Texture2D,
			ResourceType_Texture3D,
			ResourceType_Texture1D,
			ResourceType_Unknown,
		};

#pragma pack(1)

		struct AssetHeader
		{
			char mFlag[4];
			uint32_t mAvaliableFlags;
		};

		struct AssetInfo
		{
			uint32_t mOffset;
			uint32_t mSize;
			ResourceType mType;
			uint8_t mReserve[20];
		};
#pragma pack()

		class Asset : public IDisposable
		{
		protected:
			virtual ~Asset() {}
		public:
			static SKULD_EXPORT Asset* Open(Stream* mStream);
			static SKULD_EXPORT Asset* Create(Stream* mStream, const String& mName, uint32_t mCheckFlag = 0xffffffff);

			virtual Asset* Repack(Stream* mStream) = 0;
			virtual void RemoveFile(const String& mPath) = 0;
			virtual const AssetInfo * AddFile(const String & mPath,
				const String & mFile, ResourceType mType = ResourceType_Unknown) = 0;
			virtual const AssetInfo * AddFile(const String & mPath,
				const uint8_t * mData, size_t mDataSize, ResourceType mType = ResourceType_Unknown) = 0;
			virtual const AssetInfo * AddFile(const String & mPath,
				Stream * mStream, size_t mDataSize, ResourceType mType = ResourceType_Unknown) = 0;

			virtual const AssetHeader * Header() const = 0;
			virtual const AssetInfo * FileInfo(const String & mPath) const = 0;
			virtual Stream * GetAssetReadStream(const String & mPath) = 0;

			virtual String GetName() const = 0;
			virtual CheckFlags GetCheckFlags() const = 0;
		};
	}
}