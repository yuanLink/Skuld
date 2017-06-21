#pragma once

#include "../String.h"
#include "../Stream.h"
#include "../MathStruct.h"
#include "../Bitmap.h"
#include "UIFont.h"
#include "UIImage.h"

namespace Skuld
{
	namespace UI
	{
		class UIControl;
		class UIContext : public IDisposable
		{
		protected:
			virtual ~UIContext(){}
		public:
			virtual void FillRect(float x, float y, float width, float height, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f)) = 0;
			virtual void DrawRect(float x, float y, float width, float height, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f), double stroke = 1.0) = 0;
			virtual void DrawEllipse(float x, float y, float width, float height, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f), double stroke = 1.0) = 0;
			virtual void FillEllipse(float x, float y, float width, float height, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f)) = 0;
			virtual void DrawLine(float x, float y, float end_x, float end_y, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f), double stroke = 1.0) = 0;

			virtual const uint8_t* GetPixelData() const = 0;
			virtual const int32_t GetWidth() const = 0;
			virtual const int32_t GetHeight() const = 0;
			virtual void Resize(uint32_t mWidth, uint32_t mHeight, bool mErase) = 0;

			virtual UIFont* CreateUIFont(Stream* stream) = 0;
			virtual UIFont* GetDefaultFont() = 0;
			
			virtual UIImage* CreateUIImage(const Bitmap* bmp) = 0;
			virtual void DrawImage(float x, float y, UIImage* image) = 0;

			virtual void DrawString(float x, float y, const String& str, float font_size, UIFont* font = nullptr, const float4& color = float4(0.0f, 0.0f, 0.0f, 1.0f)) = 0;
		};
	}
}