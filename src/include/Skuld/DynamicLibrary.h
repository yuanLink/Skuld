#pragma once
#include "String.h"
#include "IDisposable.h"
#include <functional>

namespace Skuld
{
	class DynamicLibrary : public IDisposable
	{
	protected:
		virtual ~DynamicLibrary() {}
		virtual void* GetSymbol_(const String& mSymbol) const = 0;
	public:
		template<typename T>
		std::function<T> GetSymbol(const String& mSymbol) const {
			void* mHandle = GetSymbol_(mSymbol);
			return ((T*)mHandle);
		}

		static SKULD_EXPORT DynamicLibrary* OpenDynamicLibrary(const String& mPath);
	};
}