#include <stdint.h>

namespace Skuld
{
	namespace UI
	{
		class UI;
		class UIContext;
		UIContext* CreateCairoContext(uint32_t mWidth, uint32_t mHeight);
		UI* CreateUI(UIContext* mContext);
	}
}