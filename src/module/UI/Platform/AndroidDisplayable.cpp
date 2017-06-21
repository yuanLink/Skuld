#include <Skuld/UI/Displayable.h>
#include <WindowInfo.h>

namespace Skuld
{
	namespace UI
	{
		class AndroidDisplayable : public Displayable
		{
		protected:
			virtual ~AndroidDisplayable();
		private:
			WindowInfo mInfo;
		public:
			const WindowInfo* GetWindowInfo() const;
			AndroidDisplayable(ANativeWindow* window) { mInfo.window = window; }
			uint32_t GetWidth();
			uint32_t GetHeight();
			bool MainLoop();
		};

		Displayable* Displayable::Create(ANativeWindow *window)
		{
			ANativeWindow_acquire(window);
			return new AndroidDisplayable(window);
		}

		Displayable* Create(JNIEnv* env, jobject surface)
		{
			return new AndroidDisplayable(ANativeWindow_fromSurface(env, surface));
		}

		const WindowInfo* AndroidDisplayable::GetWindowInfo() const { return &mInfo; }

		uint32_t AndroidDisplayable::GetWidth()
		{
			return static_cast<uint32_t>(ANativeWindow_getWidth(this->mInfo.window));
		}

		uint32_t AndroidDisplayable::GetHeight()
		{
			return static_cast<uint32_t>(ANativeWindow_getWidth(this->mInfo.window));
		}

		bool AndroidDisplayable::MainLoop()
		{
			return false;
		}

		AndroidDisplayable::~AndroidDisplayable()
		{
			ANativeWindow_release(this->mInfo.window);
		}
	}
}