#pragma once

#include "Stream.h"
#include "String.h"

namespace Skuld
{
	enum FileMode
	{
		ReadOnly,
		Open,
		Create,
		OpenOrCreate
	};

	class FileStream : public Stream
	{
	protected:
		virtual ~FileStream() {}
		FileStream(const FileStream&&) = delete;
		FileStream& operator=(const FileStream&) = delete;
		FileStream() {};
	public:
		static SKULD_EXPORT FileStream* Open(const String& str, FileMode mode);
		virtual String GetPath() const = 0;
	};
}