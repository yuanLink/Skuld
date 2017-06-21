#include <Skuld/Stream.h>
#include <Skuld/Exception.h>
#include <Skuld/Ptr.hpp>
#include <map>
#include <array>
#include <mutex>
#include "../Support/MathLib.h"
#include "AssetsInternal.h"

namespace Skuld
{
	namespace Storage
	{
		class AssetStream : public Stream
		{
		protected:
			virtual ~AssetStream() {
			}

			size_t mOffset, mSize;

			size_t mPosition = 0;
			Ptr<AssetImpl> mAssets;
		public:
			AssetStream(size_t mOffset, size_t mSize, AssetImpl* mAssets) :
				mOffset(mOffset), mSize(mSize), mAssets(mAssets)
			{
				mAssets->AddRef();
			}

			virtual size_t Read(void* buffer, size_t size) {
				std::lock_guard<std::mutex> mLock(this->mAssets->mMutex);

				this->mAssets->mStream->Seek(this->mOffset + mPosition);
				size_t mRet = this->mAssets->mStream->Read(buffer, Min(size, this->mSize - mPosition));
				mPosition += mRet;
				return mRet;
			}
			virtual size_t Write(const void* buffer, size_t size) {
				throw Exception("不能写入流");
			}
			virtual size_t Peek(void* buffer, size_t size) {
				auto current = GetPosition();
				auto ret = Read(buffer, size);
				Seek(current);
				return ret;
			}
			virtual int32_t Seek(size_t position) {
				return static_cast<int32_t>(mPosition = Max<size_t>(Min(mPosition, mSize), 0));
			}
			virtual int32_t Offset(long offset) {
				return static_cast<int32_t>(mPosition = Max<size_t>(Min(mPosition + static_cast<size_t>(offset), mSize), 0));
			}
			virtual size_t GetPosition() const { return mPosition; }
			virtual size_t GetLength() const { return mSize; }
			virtual bool CanRead() const { return true; }
			virtual bool CanWrite() const { return false; }
			virtual void Flush() {}
			virtual bool EndOfStream() const { return mPosition >= mSize; }
		};

		Stream * AssetImpl::GetAssetReadStream(const String & mPath)
		{
			String mPath2 = mPath.Replace(SKULD_STR('\\'), SKULD_STR('/'));
			std::map<String, AssetInfo>::const_iterator it = this->mInfo.find(mPath2);

			if (it == this->mInfo.end()) throw Exception("文件不存在");

			return new AssetStream(static_cast<size_t>(it->second.mOffset),
				static_cast<size_t>(it->second.mSize), this);
		}

		CheckFlags AssetImpl::GetCheckFlags() const
		{
			return this->mHeader.mAvaliableFlags;
		}
	}
}