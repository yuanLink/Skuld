#pragma once
#include "../Export.h"
#include "../IDisposable.h"
#if defined(__ANDROID__)
#include <android/native_window_jni.h>
#endif

namespace Skuld
{
	struct WindowInfo;
	namespace UI
	{
		class Displayable : public IDisposable
		{
		protected:
			virtual ~Displayable(){}
		public:
			virtual uint32_t GetWidth() = 0;
			virtual uint32_t GetHeight() = 0;
			virtual bool MainLoop() = 0;
			virtual const WindowInfo* GetWindowInfo() const = 0;
#if defined(__ANDROID__)
			SKULD_EXPORT static Displayable* Create(ANativeWindow* window);
			SKULD_EXPORT static Displayable* Create(JNIEnv* env, jobject surface);
#elif defined(_WIN32) || defined(__linux__) || defined(__MACOSX__)
			SKULD_EXPORT static Displayable* Create(uint32_t width, uint32_t height);
#endif
		};
	}
}