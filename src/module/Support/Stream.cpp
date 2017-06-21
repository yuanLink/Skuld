#include <Skuld/Stream.h>
#include "string.h"
#include <memory>

namespace Skuld
{
	String Stream::ReadString()
	{
		uint32_t count;
		this->Read(&count, 4);
		std::unique_ptr<char[]> buf = std::make_unique<char[]>(count);
		this->Read(buf.get(), count);
		return String(buf.get(), false);
	}

	void Stream::WriteString(const String& str)
	{
		const std::string buf = str.GetUTF8Str();
		uint32_t size = static_cast<uint32_t>(buf.size()) + 1;
		this->Write(&size, 4);
		this->Write(buf.c_str(), size);
	}

	void Stream::Close()
	{
		Dispose();
	}
}