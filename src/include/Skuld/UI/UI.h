#pragma once
#include "UIContext.h"
#include "UIControl.h"

namespace Skuld
{
	namespace UI
	{
		class UI : public IDisposable
		{
		protected:
			virtual ~UI();
		protected:
			UIContext* pContext;
			virtual void AddControl(UIControl* pControl) = 0;
			UI(UIContext* pContext);
		public:
			virtual void OnPaint() = 0;

			virtual void DestroyControl(UIControl* c) = 0;
			virtual const UIContext* GetContext() const = 0;
			virtual void SetContext(UIContext* c) = 0;

			template<class T> T* CreateControl() {
				T* ret = new (std::nothrow) T();
				this->AddControl(ret);
				return ret;
			}
		};
	}
}