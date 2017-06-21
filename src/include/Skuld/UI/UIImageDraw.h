#pragma once
#include "UIControl.h"

namespace Skuld
{
	namespace UI
	{
		class UIImageDraw : public UIDraw
		{
		public:
			UIImageDraw(const UIContext* pContext) : UIDraw(pContext) {}
			virtual void Draw(const UIContext* const pContext, float x, float y, float width, float height);
			virtual void SetImage(const UIImage* img);
			virtual void SetWrapMode();
		};
	}
}