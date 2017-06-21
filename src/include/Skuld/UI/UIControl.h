#pragma once

#include "../IDisposable.h"
#include "UIContext.h"
#include "UIInput.h"

namespace Skuld
{
	namespace UI
	{
		class SKULD_EXPORT UIDraw
		{
		protected:
			const UIContext* pContext;
		public:
			UIDraw(const UIContext* pContext) : pContext(pContext) {}
			virtual void Draw(const UIContext* const pContext, float x, float y, float width, float height) const = 0;
		};

		class SKULD_EXPORT UIControl : public IDisposable
		{
		private:
			float size_x, size_y;
			bool focus;
			bool visible;
			float x, y, z;
			const UIDraw* pDraw;
		protected:
			void SetBackground(const UIDraw* const pDraw);
			virtual ~UIControl() {}
			friend class UI;
			virtual void OnDispatchEvent(const UIInput* const pInput);
		public:
			virtual bool OnCreate(const UIContext* const pContext) = 0;
			virtual bool OnDestroy(const UIContext* const pContext) = 0;

			virtual bool OnPaint(UIContext* pContext);

			virtual bool OnGetFocus(const UIContext* const pContext);
			virtual bool OnLostFocus(const UIContext* const pContext);

			virtual void SetPosition(const float3& pos);
			virtual void SetPosition(float x, float y, float z = 0.f);
			virtual float3 GetPosition() const;

			virtual void SetWidth(float w);
			virtual void SetHeight(float w);
			virtual float GetWidth() const;
			virtual float GetHeight() const;
			virtual void SetVisible(bool v);
			virtual bool GetVisible() const;

			bool IsContain(const float2& point) const;
		};
	}
}