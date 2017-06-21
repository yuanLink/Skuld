#pragma once
#include <stddef.h>
#include "String.h"
#include "IDisposable.h"

namespace Skuld
{
	class SKULD_EXPORT Stream : public IDisposable
	{
	protected:
		virtual ~Stream() {}
	public:
		virtual size_t Read(void* buffer, size_t size) = 0;
		virtual size_t Write(const void* buffer, size_t size) = 0;
		virtual void Close();
		String ReadString();
		void WriteString(const String& str);
		virtual size_t Peek(void* buffer, size_t size) = 0;
		virtual int32_t Seek(size_t position) = 0;
		virtual int32_t Offset(long offset) = 0;
		virtual size_t GetPosition() const = 0;
		virtual size_t GetLength() const = 0;
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		virtual void Flush() = 0;
		virtual bool EndOfStream() const = 0;
		template<typename T> void Read(T* ret) {
			this->Read(ret, sizeof(T));
		}
		template<typename T> void Write(T* obj) {
			this->Write(obj, sizeof(T));
		}
	};
}