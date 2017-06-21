#include "UICairoContext.h"
#include "../Support/MathLib.h"
#include "UICairoImage.h"
#include "UIInternal.h"

namespace Skuld
{
	namespace UI
	{
		UIContext* CreateCairoContext(uint32_t mWidth, uint32_t mHeight)
		{
			cairo_surface_t* mSurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32,
				static_cast<int>(mWidth), static_cast<int>(mHeight));
			return new UICairoContext(mSurface);
		}

		UICairoContext::~UICairoContext()
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			cairo_surface_destroy(mSurface);
		}

		void UICairoContext::DrawRect(float x, float y, float width, float height, const float4 & color, double stroke)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_line_width(cr, stroke);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke(cr);
			cairo_destroy(cr);
		}

		void UICairoContext::FillRect(float x, float y, float width, float height, const float4 & color)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			cairo_rectangle(cr, x, y, width, height);
			cairo_fill(cr);
			cairo_destroy(cr);
		}

		void UICairoContext::DrawEllipse(float x, float y, float width, float height, const float4 & color, double stroke)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_line_width(cr, stroke);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			float r = Max(width, height);
			float xs = width / r;
			float ys = height / r;
			cairo_scale(cr, xs, ys);
			cairo_arc(cr, x, y, r / 2, 0, 2 * g_PI);
			cairo_stroke(cr);
			cairo_destroy(cr);
		}

		void UICairoContext::FillEllipse(float x, float y, float width, float height, const float4 & color)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			float r = Max(width, height);
			float xs = width / r;
			float ys = height / r;
			cairo_scale(cr, xs, ys);
			cairo_arc(cr, x, y, r / 2, 0, 2 * g_PI);
			cairo_fill(cr);
			cairo_destroy(cr);
		}

		const uint8_t * UICairoContext::GetPixelData() const
		{
			return cairo_image_surface_get_data(mSurface);
		}

		void UICairoContext::DrawLine(float x, float y, float end_x, float end_y, const float4 & color, double stroke)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_line_width(cr, stroke);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			cairo_move_to(cr, x, y);
			cairo_line_to(cr, end_x, end_y);
			cairo_stroke(cr);
			cairo_destroy(cr);
		}

		const int32_t UICairoContext::GetHeight() const
		{
			return cairo_image_surface_get_height(mSurface);
		}

		UIFont * UICairoContext::CreateUIFont(Stream * stream)
		{
			return new (std::nothrow) UICairoFont(this, stream);
		}

		void UICairoContext::DrawImage(float x, float y, UIImage* image)
		{
			if (image == nullptr) throw Exception("image不能为nullptr");
			if (image->GetContext() != this)
				throw Exception("请使用这个UIContext对象创建的UIFont");
			UICairoImage* pImage = static_cast<UICairoImage*>(image);

			std::lock_guard<std::mutex> mLock(this->mMutex);

			cairo_t * cr = cairo_create(mSurface);
			cairo_move_to(cr, x, y);

			cairo_set_source_surface(cr, pImage->mSurface, 0, 0);
			cairo_paint(cr);

			cairo_destroy(cr);
		}

		UIImage * UICairoContext::CreateUIImage(const Bitmap * bmp)
		{
			cairo_format_t mFormat;
			uint32_t stride, width = bmp->GetWidth(), height = bmp->GetHeight();

			mFormat = cairo_format_t::CAIRO_FORMAT_ARGB32;

			stride = 4 * width;

			Bitmap* mBuffer = bmp->Convert(PixelFormat_RGBA_8888);

			cairo_surface_t *mSurface = cairo_image_surface_create_for_data(mBuffer->GetPixels(), 
				mFormat, width, height, stride);

			return new UICairoImage(mSurface, this, mBuffer);
		}

		const int32_t UICairoContext::GetWidth() const
		{
			return cairo_image_surface_get_width(mSurface);
		}

		void UICairoContext::Resize(uint32_t mWidth, uint32_t mHeight, bool mErase)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			cairo_surface_t* mNewSurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32,
				static_cast<int>(mWidth), static_cast<int>(mHeight));

			if (!mErase)
			{
				int mOldWidth = cairo_image_surface_get_width(mSurface);
				int mOldHeight = cairo_image_surface_get_height(mSurface);
				cairo_t * cr = cairo_create(mNewSurface);
				cairo_set_source_surface(cr, mSurface, 0, 0);
				cairo_rectangle(cr, 0, 0, Min<double>(mOldWidth, mWidth), Min<double>(mOldHeight, mHeight));
				cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
				cairo_fill(cr);
				cairo_destroy(cr);
			}

			cairo_surface_destroy(mSurface);
			mSurface = mNewSurface;
		}

		void UICairoContext::DrawString(float x, float y, const String & str, float font_size,
			UIFont * font, const float4& color)
		{
			if (font != nullptr && font->GetContext() != this)
				throw Exception("请使用这个UIContext对象创建的UIFont");
			if (font == nullptr && mDefaultFont == nullptr)
				throw Exception("无法加载字体");
			UICairoFont* pFont = (font != nullptr) ? static_cast<UICairoFont*>(font) : mDefaultFont;

			cairo_font_face_t* mCairoFace = pFont->mCairoFace;

			cairo_t * cr = cairo_create(mSurface);
			cairo_set_source_rgba(cr, color.x, color.y, color.z, color.w);
			cairo_set_font_face(cr, mCairoFace);
			cairo_set_font_size(cr, font_size);

			cairo_text_extents_t mExtents;
			double new_x, new_y;

			std::string utf8str = str.GetUTF8Str();
			cairo_text_extents(cr, utf8str.c_str(), &mExtents);

			new_x = x;
			new_y = y + mExtents.height;

			cairo_move_to(cr, new_x, new_y);
			cairo_show_text(cr, utf8str.c_str());

			cairo_destroy(cr);
		}

		void UICairoContext::DrawComplexString(float x, float y, const String & str, 
			float font_size, UIFont * font, const float4 & color)
		{
			if (font != nullptr && font->GetContext() != this)
				throw Exception("请使用这个UIContext对象创建的UIFont");
			if (font == nullptr && mDefaultFont == nullptr)
				throw Exception("无法加载字体");
			UICairoFont* pFont = (font != nullptr) ? static_cast<UICairoFont*>(font) : mDefaultFont;

			hb_font_t* mHarfbuzzFace = pFont->mHarfbuzzFace;
			cairo_font_face_t* mCairoFace = pFont->mCairoFace;
			
			hb_buffer_t * mBuffer = hb_buffer_create();

			std::string utf8str = str.GetUTF8Str();
			hb_buffer_add_utf8(mBuffer, utf8str.c_str(), (int)utf8str.size(), 0, -1);

			hb_shape(mHarfbuzzFace, mBuffer, nullptr, 0);
			unsigned int count = hb_buffer_get_length(mBuffer);
			hb_glyph_info_t* infos = hb_buffer_get_glyph_infos(mBuffer, nullptr);
			hb_glyph_position_t* positions = hb_buffer_get_glyph_positions(mBuffer, nullptr);

			for (unsigned int i = 0; i < count; i++)
			{
				hb_glyph_info_t* info = &infos[i];
				hb_glyph_position_t* pos = &positions[i];


			}

			hb_buffer_destroy(mBuffer);
		}
	}
}