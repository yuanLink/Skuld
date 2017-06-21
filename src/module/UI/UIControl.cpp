#include <Skuld/UI/UIControl.h>

namespace Skuld
{
	namespace UI
	{
		bool UIControl::OnPaint(UIContext * pContext)
		{
			if (this->pDraw)
				this->pDraw->Draw(pContext, this->x, this->y, this->size_x, this->size_y);
			return true;
		}

		bool UIControl::OnGetFocus(const UIContext * const pContext)
		{
			this->focus = true;
			return true;
		}

		bool UIControl::OnLostFocus(const UIContext * const pContext)
		{
			this->focus = false;
			return true;
		}

		void UIControl::SetPosition(const float3 & pos)
		{
			this->x = pos.x;
			this->y = pos.y;
			this->z = pos.z;
		}

		void UIControl::SetPosition(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		float3 UIControl::GetPosition() const
		{
			return float3(x, y, z);
		}

		void UIControl::SetWidth(float w)
		{
			this->size_x = w;
		}

		void UIControl::SetHeight(float w)
		{
			this->size_y = w;
		}

		float UIControl::GetWidth() const
		{
			return this->size_x;
		}

		float UIControl::GetHeight() const
		{
			return this->size_y;
		}

		void UIControl::SetVisible(bool v)
		{
			this->visible = v;
		}

		bool UIControl::GetVisible() const
		{
			return this->visible;
		}

		bool UIControl::IsContain(const float2 & point) const
		{
			bool x = (point.x >= this->x && point.x <= (this->x + this->size_x));
			bool y = (point.y >= this->y && point.y <= (this->y + this->size_y));
			return (x && y);
		}

		void UIControl::SetBackground(const UIDraw * const _pDraw)
		{
			this->pDraw = _pDraw;
		}

		void UIControl::OnDispatchEvent(const UIInput * const pInput)
		{
			//TODO
		}
	}
}