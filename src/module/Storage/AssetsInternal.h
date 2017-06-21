#pragma once

#include <Skuld/Storage/libAssets.h>
#include <Skuld/Ptr.hpp>
#include <map>
#include <mutex>

#define ResourceType_Max ResourceType_Unknown + 1

namespace Skuld
{
	namespace Storage
	{
#pragma pack(1)
		struct AssetHeaderImpl : AssetHeader
		{
			uint32_t mInfoOffset;
			uint32_t mFileCount;
			uint32_t mVersion;
			char mReserve[12];
		};
#pragma pack()

		class AssetImpl : public Asset
		{
		protected:
			virtual ~AssetImpl();
			Ptr<Stream> mStream;

			String mName;

			AssetHeaderImpl mHeader;

			std::map<String, AssetInfo> mInfo;

			bool mHasChanged = false;

			friend class Asset;

			std::mutex mMutex;
			friend class AssetStream;
		public:
			virtual String GetName() const;
			virtual const AssetHeader * Header() const;
			virtual const AssetInfo * FileInfo(const String & mPath) const;
			virtual Asset* Repack(Stream* mStream);
			virtual void RemoveFile(const String& mPath);
			virtual Stream * GetAssetReadStream(const String & mPath);

			virtual CheckFlags GetCheckFlags() const;

			virtual const AssetInfo * AddFile(const String & mPath,
				Stream * mStream, size_t mDataSize, ResourceType mType = ResourceType_Unknown);
			virtual const AssetInfo * AddFile(const String & mPath, const String & mFile, ResourceType mType);
			virtual const AssetInfo * AddFile(const String & mPath, const uint8_t * mData, size_t mDataSize, ResourceType mType);
		};
	}
}