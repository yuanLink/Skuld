#pragma once

#include <functional>

namespace Skuld
{
	template<size_t mCacheSize>
	class WriteCache
	{
	private:
		WriteCache() = delete;
		WriteCache(WriteCache&) = delete;

		uint8_t mCache[mCacheSize];
		size_t mCurrentPos = 0;
		std::function<size_t(const uint8_t*, size_t)> mCallback;
	public:
		~WriteCache() { }
		WriteCache(std::function<size_t(const uint8_t*, size_t)> mCallback) :
			mCallback(mCallback) {};
		size_t Flush() {
			size_t ret = mCallback(mCache, mCurrentPos);
			mCurrentPos = 0;
			return ret;
		}
		size_t Flush(std::function<size_t(const uint8_t*, size_t)> mCallback2) {
			size_t ret = mCallback2(mCache, mCurrentPos);
			mCurrentPos = 0;
			return ret;
		}
		size_t Write(const void* mBuffer, size_t mSize) {
			size_t mWriteSize = 0;
			if (mSize + mCurrentPos >= mCacheSize)
			{
				if (mCurrentPos != 0)
				{
					memcpy(&mCache[mCurrentPos], mBuffer, mCacheSize - mCurrentPos);
					mCallback(mCache, mCacheSize);
					mWriteSize += mCacheSize - mCurrentPos;
				}

				if (mSize - mWriteSize > mCacheSize)
					mWriteSize += mCallback(((uint8_t*)mBuffer) + mWriteSize, mSize - mWriteSize);
				else memcpy(mCache, ((uint8_t*)mBuffer) + mWriteSize, mSize - mWriteSize);
				mCurrentPos = mSize - mWriteSize;
			}
			else
			{
				memcpy(&mCache[mCurrentPos], mBuffer, mSize);
				mCurrentPos += mSize;
				mWriteSize = mSize;
			}
			return mWriteSize;
		}
	};
}