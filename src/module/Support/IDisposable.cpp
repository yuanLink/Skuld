#include <Skuld/IDisposable.h>

namespace Skuld
{
	IDisposable::~IDisposable() {

	}

	void IDisposable::Dispose()
	{
		mRefCount--;
		if (mRefCount == 0)
			delete this;
	}

	uint32_t IDisposable::RefCount()
	{
		return mRefCount;
	}

	void IDisposable::AddRef()
	{
		mRefCount++;
	}
}