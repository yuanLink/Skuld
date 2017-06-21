#include <Skuld/UI/UIButton.h>

namespace Skuld
{
	namespace UI
	{
		UIButton::~UIButton() {}

		bool UIButton::OnCreate(const UIContext * const pContext)
		{
			return true;
		}

		bool UIButton::OnDestroy(const UIContext * const pContext)
		{
			return true;
		}

		bool UIButton::OnPaint(UIContext * pContext)
		{
			this->UIControl::OnPaint(pContext);
			//float3 pos = this->GetPosition();
			//UIFont* font = _font == nullptr ? pContext->GetDefaultFont() : _font;
			//float2 size;// = font->CalcSize(this->text);
			//
			//pContext->DrawString(pos.x + (this->GetWidth() - size.x) / 2,
			//	pos.y + this->GetHeight() / 2, this->text);
			return true;
		}

		void UIButton::SetPressedDraw(const UIDraw * const pDraw)
		{
			this->pPressed = pDraw;
		}

		void UIButton::SetUnPressedDraw(const UIDraw * const pDraw)
		{
			this->pUnpressed = pDraw;
		}

		void UIButton::SetText(const String & str)
		{
			this->text = str;
		}

		void UIButton::SetFont(UIFont * font)
		{
			this->_font = font;
		}

		UIFont * UIButton::GetFont() const
		{
			return this->_font;
		}

		String UIButton::GetText() const
		{
			return this->text;
		}
	}
}