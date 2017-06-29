#include <Skuld/Text.h>
#include <Skuld/FileStream.h>
#include <Skuld/Ptr.hpp>
#include <vector>

namespace Skuld
{
	static const uint32_t UTF8BOM = 0xbfbbef;
	static const uint32_t UTF16BEBOM = 0xfffe;
	static const uint32_t UTF16LEBOM = 0xfeff;
	static const uint32_t UTF32BEBOM = 0xfffe0000;
	static const uint32_t UTF32LEBOM = 0x0000feff;

	String ReadAllTextFromFile(const String & mFile)
	{
		Ptr<FileStream> mFileStream = FileStream::Open(mFile, Open);

		if (mFileStream == nullptr) return "";
	
		return std::move(ReadAllTextFromStream(mFileStream));
	}

	String ReadAllTextFromStream(Stream * mStream)
	{
		uint32_t mFlag;
		mStream->Peek(&mFlag, 4);

		size_t mLength = mStream->GetLength();

		if ((mFlag & 0xffffff) == UTF8BOM)
		{
			std::vector<uint8_t> mBuffer(mLength + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mLength] = 0;

			return String((char*)&mBuffer[3], false);
		}
		else if (mFlag == UTF32LEBOM)
		{
			std::vector<char32_t> mBuffer((mLength / 4) + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mBuffer.size() - 1] = 0;

			return String(&mBuffer[1]);
		}
		else if (mFlag == UTF32BEBOM)
		{
			std::vector<char32_t> mBuffer((mLength / 4) + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mBuffer.size() - 1] = 0;

			for (size_t i = 1; i < mBuffer.size(); i++)
			{
				mBuffer[i] =
					((mBuffer[i] & 0x000000ff) << 24) |
					((mBuffer[i] & 0x0000ff00) << 8) |
					((mBuffer[i] & 0x00ff0000) >> 8) |
					((mBuffer[i] & 0xff000000) >> 24 );
			}

			return String(&mBuffer[1]);
		}
		else if (mFlag == UTF16LEBOM)
		{
			std::vector<char16_t> mBuffer((mLength / 2) + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mBuffer.size() - 1] = 0;

			return String(&mBuffer[1]);
		}
		else if (mFlag == UTF16BEBOM)
		{
			std::vector<char16_t> mBuffer((mLength / 2) + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mBuffer.size() - 1] = 0;

			for (size_t i = 1; i < mBuffer.size(); i++)
			{
				mBuffer[i] =
					((mBuffer[i] & 0x00ff) << 8) |
					((mBuffer[i] & 0xff00) >> 8);
			}

			return String(&mBuffer[1]);
		}
		else
		{
			std::vector<char> mBuffer(mLength + 1);
			mStream->Read(mBuffer.data(), mLength);
			mBuffer[mLength] = 0;

			return String(mBuffer.data());
		}
	}
}