#pragma once
#include "Export.h"

namespace Skuld
{
	class SKULD_EXPORT IDisposable
	{
	private:
		uint32_t mRefCount = 1;
	protected:
		virtual ~IDisposable() = 0;
	public:
		virtual void Dispose();
		virtual void AddRef();
		virtual uint32_t RefCount();
	};
}