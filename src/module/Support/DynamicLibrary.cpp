#include <Skuld/DynamicLibrary.h>

#ifdef _WIN32
#include <Windows.h>
typedef HMODULE LibraryHandle;
#define libopen(b) LoadLibraryW(b.GetWStr().c_str())
#define libsym(a, b) GetProcAddress((HMODULE)a, b)
#define libclose(a) FreeLibrary((HMODULE)a)
#else
#include <dlfcn.h>
typedef void* LibraryHandle;
#define libopen(b) dlopen(b.GetStr().c_str(), RTLD_LAZY)
#define libsym(a, b) dlsym(a, b)
#define libclose(a) dlclose(a)
#endif

namespace Skuld
{
	class DynamicLibraryImpl : public DynamicLibrary
	{
	protected:
		virtual ~DynamicLibraryImpl();
		LibraryHandle mHandle;
	public:
		DynamicLibraryImpl(LibraryHandle mHandle) : mHandle(mHandle) {}
		virtual void* GetSymbol_(const String& mSymbol) const;
	};

	DynamicLibraryImpl::~DynamicLibraryImpl()
	{
		libclose(mHandle);
	}

	void * DynamicLibraryImpl::GetSymbol_(const String & mSymbol) const
	{
		return libsym(mHandle, mSymbol.GetStr().c_str());
	}

	DynamicLibrary * DynamicLibrary::OpenDynamicLibrary(const String & mPath)
	{
		return new (std::nothrow) DynamicLibraryImpl(libopen(mPath));
	}
}