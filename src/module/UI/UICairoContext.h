#pragma once
#include <Skuld/UI/UIContext.h>
#include <Skuld/FileStream.h>
#include <Skuld/GlobalConfig.h>
#include <cairo.h>
#include <cairo-ft.h>
#include <mutex>
#include <Skuld/Ptr.hpp>
#include <Skuld/Exception.h>
#include "UICairoFont.h"

namespace Skuld
{
	namespace UI
	{
		class UICairoContext : public UIContext
		{
		private:
			std::mutex mMutex;
			cairo_surface_t* mSurface;

			UICairoFont* mDefaultFont = nullptr;
		public:
			UICairoContext(cairo_surface_t* mSurface) : mSurface(mSurface) {
				auto fs = MakePtr(FileStream::Open(
					GetGlobalConfig()->GetDefaultFontPath(), FileMode::Open));
				if (fs != nullptr)
					this->mDefaultFont = new UICairoFont(this, fs);
			}
			~UICairoContext();
			void DrawRect(float x, float y, float width, float height, const float4& color, double stroke);
			void FillRect(float x, float y, float width, float height, const float4& color);
			void DrawEllipse(float x, float y, float width, float height, const float4& color, double stroke);
			void FillEllipse(float x, float y, float width, float height, const float4& color);
			const uint8_t* GetPixelData() const;
			void DrawLine(float x, float y, float end_x, float end_y, const float4& color, double stroke);
			const int32_t GetWidth() const;
			const int32_t GetHeight() const;
			UIFont* CreateUIFont(Stream* stream);

			void DrawImage(float x, float y, UIImage* image);
			UIImage* CreateUIImage(const Bitmap* bmp);

			UIFont* GetDefaultFont() { return this->mDefaultFont; }
			void Resize(uint32_t mWidth, uint32_t mHeight, bool mErase);
			void DrawString(float x, float y, const String& str, float font_size, UIFont* font, const float4& color);

			void DrawComplexString(float x, float y, const String& str, float font_size, UIFont* font, const float4& color);
		};
	}
}