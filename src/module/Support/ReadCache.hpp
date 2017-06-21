#pragma once

#include <functional>
#include "MathLib.h"

namespace Skuld
{
	template<size_t mCacheSize>
	class ReadCache
	{
	private:
		ReadCache() = delete;
		ReadCache(ReadCache&) = delete;

		uint8_t mCache[mCacheSize];
		size_t mCurrentPos;
		size_t mCurrentCacheSize = mCacheSize;
		std::function<size_t(uint8_t*, size_t)> mCallback;
		bool mEof = false;
	public:
		~ReadCache() { }
		ReadCache(std::function<size_t(uint8_t*, size_t)> mCallback) :
			mCallback(mCallback), mCurrentPos(mCacheSize) {};

		void Reset()
		{
			mCurrentCacheSize = mCacheSize;
			mEof = false;
			mCurrentPos = mCacheSize;
		}

		size_t Read(uint8_t *mBuffer, size_t mSize) {
			if (!mEof)
			{
				size_t mCurrentRest = mCurrentCacheSize - mCurrentPos;
				size_t mReadSize = 0;
				if (mCurrentRest != 0)
					memcpy(mBuffer, &mCache[mCurrentPos], mCurrentRest);

				if (mCurrentRest < mSize)
				{
					size_t temp = mCallback(mBuffer + mReadSize, mSize - mCurrentRest);
					if (temp < mSize - mCurrentRest)
					{
						mEof = true;
						mCurrentCacheSize = 0;
					}

					mReadSize += temp;
					mCurrentPos = mCurrentCacheSize;
				}
				else
				{
					mReadSize = mCurrentRest;
					mCurrentPos += mReadSize;
				}
				return mReadSize;
			}
			else
			{
				size_t temp = Min(mCurrentCacheSize, mSize);
				if (temp != 0)
				{
					memcpy(mBuffer, &mCache[mCurrentPos], temp);
					mCurrentPos += temp;
				}
				return temp;
			}
		}
	};
}