#include <Skuld/FileStream.h>
#include <Skuld/String.h>
#include <Skuld/Path.h>
#include <stdio.h>
#include <memory>
#include "ReadCache.hpp"
#include "WriteCache.hpp"
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

#ifdef WIN32
#define fseeko _fseeki64
#define ftello _ftelli64
#endif

#define SKULD_FILESTREAM_CACHE_SIZE 16 * 1024

namespace Skuld
{
	class InternalFileStream : public FileStream
	{
	public:
		FILE* handle;
		String path;
		std::unique_ptr<WriteCache<SKULD_FILESTREAM_CACHE_SIZE>> mWriteCache;
		std::unique_ptr<ReadCache<SKULD_FILESTREAM_CACHE_SIZE>> mReadCache;
		size_t mPosition = 0;

		InternalFileStream(FILE* handle, const String& path) : handle(handle), path(path) {
			mWriteCache = std::make_unique<WriteCache<SKULD_FILESTREAM_CACHE_SIZE>>(
				[this](const uint8_t* buf, size_t s) {
				return fwrite(buf, 1, s, this->handle);
			});
			mReadCache = std::make_unique<ReadCache<SKULD_FILESTREAM_CACHE_SIZE>>(
				[this](uint8_t* buf, size_t s) {
				return fread(buf, 1, s, this->handle);
			});
		}
		~InternalFileStream();

		size_t Read(void* buffer, size_t size);
		size_t Write(const void* buffer, size_t size);
		int32_t Seek(size_t position);
		int32_t Offset(long offset);
		size_t GetPosition() const;
		bool CanRead() const;
		bool CanWrite() const;
		size_t GetLength() const;
		void Flush();
		bool EndOfStream() const;
		size_t Peek(void* buffer, size_t size);

		String GetPath() const { return this->path; }
	};

	size_t InternalFileStream::Peek(void* buffer, size_t size)
	{
		auto current = GetPosition();
		auto ret = Read(buffer, size);
		Seek(current);
		return ret;
	}

	size_t InternalFileStream::Read(void * buffer, size_t size)
	{
		if (CanRead())
		{
			size_t ret = mReadCache->Read((uint8_t*)buffer, size);
			mPosition += ret;
			return ret;
		}
		return (size_t)-1;
	}

	size_t InternalFileStream::Write(const void* buffer, size_t size)
	{
		if (CanWrite())
		{
			size_t ret = mWriteCache->Write(buffer, size);
			mPosition += ret;
			return ret;
		}
		return (size_t)-1;
	}

	InternalFileStream::~InternalFileStream()
	{
		Flush();
		fclose(this->handle);
	}

	int32_t InternalFileStream::Seek(size_t position)
	{
		mPosition += mWriteCache->Flush();
		mReadCache->Reset();
		mPosition = position;
		return fseeko((FILE*)this->handle, position, 0);
	}

	int32_t InternalFileStream::Offset(long offset)
	{
		if (offset == 0) return 0;
		mPosition += mWriteCache->Flush();
		mReadCache->Reset();
		mPosition += offset;
		return fseeko(this->handle, offset, 1);
	}

	size_t InternalFileStream::GetLength() const
	{
		auto current = ftello(this->handle);
		fseeko(this->handle, 0, 2);
		auto ret = ftello(this->handle);
		fseeko(this->handle, current, 0);
		return ret;
	}

	size_t InternalFileStream::GetPosition() const
	{
		return mPosition;
	}

	bool InternalFileStream::CanRead() const
	{
		return true;
	}

	bool InternalFileStream::CanWrite() const
	{
		return true;
	}

	void InternalFileStream::Flush()
	{
		mPosition += mWriteCache->Flush();
		mReadCache->Reset();
		fflush(this->handle);
	}

	bool InternalFileStream::EndOfStream() const
	{
		return GetLength() <= GetPosition();
	}

	FileStream *FileStream::Open(const String& str, FileMode mode)
	{
		FILE* hdl;
#ifndef _WIN32
		switch (mode)
		{
		case FileMode::ReadOnly:
			hdl = fopen(str.GetStr().c_str(), "rb");
			break;
		case FileMode::Open:
			hdl = fopen(str.GetStr().c_str(), "rb+");
			break;
		case FileMode::Create:
			hdl = fopen(str.GetStr().c_str(), "wb+");
			break;
		case FileMode::OpenOrCreate: {
			auto path = str.GetStr();
			if (access(path.c_str(), 0) == -1)
			{
				hdl = fopen(path.c_str(), "wb+");
			}
			else
			{
				hdl = fopen(path.c_str(), "rb+");
			}
			break;
		}
		default:
			break;
		}
#else
		switch (mode)
		{
		case FileMode::ReadOnly:
			hdl = _wfopen(str.GetWStr().c_str(), L"rb");
			break;
		case FileMode::Open:
			hdl = _wfopen(str.GetWStr().c_str(), L"rb+");
			break;
		case FileMode::Create:
			hdl = _wfopen(str.GetWStr().c_str(), L"wb+");
			break;
		case FileMode::OpenOrCreate: {
			auto path = str.GetWStr();
			if (_waccess(path.c_str(), 0) == -1)
			{
				hdl = _wfopen(path.c_str(), L"wb+");
			}
			else
			{
				hdl = _wfopen(path.c_str(), L"rb+");
			}
			break;
		}
		default:
			break;
		}
#endif
		InternalFileStream* fs = nullptr;
		if (hdl)
			fs = new (std::nothrow) InternalFileStream(hdl, GetFullPath(str));
		return fs;
	}
}