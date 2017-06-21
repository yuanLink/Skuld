#include "AssetsInternal.h"
#include <Skuld/Exception.h>
#include <string.h>
#include <Skuld/FileStream.h>
#include "../Support/MathLib.h"

namespace Skuld
{
	namespace Storage
	{
		Asset * Asset::Create(Stream * mStream, const String& mName, uint32_t mCheckFlag)
		{
			Ptr<AssetImpl> mRet(new AssetImpl());

			AssetHeaderImpl& mHeader = mRet->mHeader;
			memset(&mHeader, 0, sizeof(AssetHeaderImpl));
			mHeader.mAvaliableFlags = mCheckFlag;
			mHeader.mFileCount = 0;
			mHeader.mInfoOffset = sizeof(AssetHeaderImpl);
			mRet->mName = mName;
			memcpy(mHeader.mFlag, "ASST", 4);

			mStream->AddRef();
			mRet->mStream = mStream;
			mRet->mHasChanged = true;
			return mRet.Detach();
		}

		static inline size_t move_data(Stream* mOut, Stream* mIn, size_t mSize)
		{
			uint8_t mCache[40960];
			size_t mWrite = 0;
			while (mWrite < mSize)
			{
				size_t mTemp = mIn->Read(mCache, Min(sizeof(mCache), mSize - mWrite));
				if (mTemp == 0) break;
				mWrite += mTemp;
				mOut->Write(mCache, mTemp);
			}
			return mWrite;
		}

		Asset * AssetImpl::Repack(Stream * mStream)
		{
			Ptr<AssetImpl> mRet(new AssetImpl());

			AssetHeaderImpl& mHeader = mRet->mHeader;
			memset(&mHeader, 0, sizeof(AssetHeaderImpl));
			mHeader.mAvaliableFlags = this->mHeader.mAvaliableFlags;
			mHeader.mFileCount = mHeader.mFileCount;
			mHeader.mInfoOffset = sizeof(AssetHeaderImpl);
			mRet->mName = this->mName;
			memcpy(mHeader.mFlag, "ASST", 4);

			mStream->AddRef();
			mRet->mStream = mStream;

			for (std::pair<const String, AssetInfo>& it : this->mInfo)
			{
				AssetInfo& mInfo = mRet->mInfo[it.first];
				mInfo.mOffset = mRet->mHeader.mInfoOffset;
				mInfo.mSize = it.second.mSize;
				mInfo.mType = it.second.mType;

				mStream->Seek(static_cast<size_t>(mInfo.mOffset));
				this->mStream->Seek(static_cast<size_t>(it.second.mOffset));

				mInfo.mSize = static_cast<uint32_t>(move_data(mStream, this->mStream, static_cast<size_t>(mInfo.mSize)));

				mRet->mHeader.mInfoOffset += mInfo.mSize - (mInfo.mSize % 0x10) + 0x10;
			}
			mRet->mHasChanged = true;
			return mRet.Detach();
		}

		void AssetImpl::RemoveFile(const String & mPath)
		{
			if (mPath == "") throw Exception("mPath不能为空");

			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			std::map<String, AssetInfo>::iterator it = this->mInfo.find(mPath2);
			if (it == this->mInfo.end()) throw Exception("文件不存在");

			this->mInfo.erase(it);
			this->mHasChanged = true;
			this->mHeader.mFileCount--;
		}

		const AssetInfo * AssetImpl::AddFile(const String & mPath, Stream * mStream, size_t mDataSize, ResourceType mType)
		{
			if (mPath == "") throw Exception("mPath不能为空");

			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			std::map<String, AssetInfo>::iterator it = this->mInfo.find(mPath2);
			if (it != this->mInfo.end()) throw Exception("文件已存在");

			AssetInfo mChunk = { 0 };
			mChunk.mType = mType;
			mChunk.mOffset = this->mHeader.mInfoOffset;

			this->mStream->Seek(static_cast<size_t>(mChunk.mOffset));

			uint8_t mCache[40960];
			size_t mWrite = 0;
			while (mWrite < mDataSize)
			{
				size_t mTemp = mStream->Read(mCache, Min(sizeof(mCache), mDataSize - mWrite));
				if (mTemp == 0) break;
				mWrite += mTemp;
				this->mStream->Write(mCache, mTemp);
			}

			mChunk.mSize = static_cast<uint32_t>(mWrite);
			this->mHeader.mInfoOffset += mChunk.mSize - (mChunk.mSize % 0x10) + 0x10;

			this->mInfo[mPath2] = mChunk;

			this->mHeader.mFileCount++;
			this->mHasChanged = true;

			return &this->mInfo[mPath2];
		}

		const AssetInfo * AssetImpl::AddFile(const String & mPath, const String & mFile, ResourceType mType)
		{
			Ptr<FileStream> mFileStream = FileStream::Open(mFile, FileMode::Open);
			return AddFile(mPath, mFileStream, mFileStream->GetLength(), mType);
		}

		const AssetInfo * AssetImpl::AddFile(const String & mPath, const uint8_t * mData, size_t mDataSize, ResourceType mType)
		{
			if (mPath == "") throw Exception("mPath不能为空");

			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			std::map<String, AssetInfo>::iterator it = this->mInfo.find(mPath2);
			if (it != this->mInfo.end()) throw Exception("文件已存在");

			AssetInfo mChunk = { 0 };
			mChunk.mSize = static_cast<uint32_t>(mDataSize);
			mChunk.mType = mType;
			mChunk.mOffset = this->mHeader.mInfoOffset;

			this->mStream->Seek(static_cast<size_t>(mChunk.mOffset));

			this->mStream->Write(mData, mDataSize);

			this->mHeader.mInfoOffset += mChunk.mSize - (mChunk.mSize % 0x10) + 0x10;

			this->mInfo[mPath2] = mChunk;

			this->mHeader.mFileCount++;
			this->mHasChanged = true;

			return &this->mInfo[mPath2];
		}

		Asset * Asset::Open(Stream * mStream)
		{
			Ptr<AssetImpl> mAsset(new AssetImpl());

			mStream->Read(&mAsset->mHeader);
			mStream->Seek(static_cast<size_t>(mAsset->mHeader.mInfoOffset));
			mAsset->mName = mStream->ReadString();

			String mPath;
			while ((mPath = mStream->ReadString()) != "")
				mStream->Read(&mAsset->mInfo[mPath]);

			mAsset->mStream = mStream;
			mStream->AddRef();
			return mAsset.Detach();
		}

		String AssetImpl::GetName() const
		{
			return mName;
		}

		const AssetHeader * AssetImpl::Header() const
		{
			return &this->mHeader;
		}

		const AssetInfo * AssetImpl::FileInfo(const String & mPath) const
		{
			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			std::map<String, AssetInfo>::const_iterator it = this->mInfo.find(mPath2);

			if (it == this->mInfo.end()) return nullptr;
			return &it->second;
		}

		AssetImpl::~AssetImpl()
		{
			if (this->mHasChanged)
			{
				this->mStream->Seek(static_cast<size_t>(this->mHeader.mInfoOffset));
				this->mStream->WriteString(this->mName);
				for (std::pair<const String, AssetInfo>& it : this->mInfo)
				{
					this->mStream->WriteString(it.first);
					this->mStream->Write(&it.second);
				}
				this->mStream->WriteString("");
				this->mStream->Seek(0);
				this->mStream->Write(&this->mHeader);
			}
		}
	}
}
