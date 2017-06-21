#pragma once

#include "UIControl.h"

namespace Skuld
{
	namespace UI
	{
		class SKULD_EXPORT UIButton : public UIControl
		{
		private:
			bool pressed = false;
			String text;
			UIFont* _font = nullptr;

			const UIDraw* pPressed;
			const UIDraw* pUnpressed;
		public:
			virtual bool OnCreate(const UIContext* const pContext);
			virtual bool OnDestroy(const UIContext* const pContext);

			virtual bool OnPaint(UIContext* pContext);

			virtual void SetPressedDraw(const UIDraw* const pDraw);
			virtual void SetUnPressedDraw(const UIDraw* const pDraw);

			virtual void SetText(const String& str);
			virtual void SetFont(UIFont* font);
			virtual UIFont* GetFont() const;
			virtual String GetText() const;

			virtual ~UIButton();
		};
	}
}