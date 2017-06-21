#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>
#include <Skuld/Stream.h>
#include <Skuld/UI/UIContext.h>
#include <cairo-ft.h>
#include <hb.h>
#include <hb-ft.h>

namespace Skuld
{
	namespace UI
	{
		static void FT_Stream_Close(FT_Stream _stream) {
			reinterpret_cast<Stream*>(_stream->descriptor.pointer)->Close();
		}

		static unsigned long FT_Stream_Read( FT_Stream stream, unsigned long offset,
			unsigned char* buffer, unsigned long count)
		{
			Stream* _stream = reinterpret_cast<Stream*>(stream->descriptor.pointer);
			if (count)
			{
				_stream->Seek(offset);
				count = static_cast<unsigned long>(_stream->Read(buffer, count));
			}
			return count;
		}

		class UIFreetypeSource
		{
		private:
			FT_StreamRec mSource;
			Stream* pStream;
		public:
			FT_StreamRec* FTSource() {
				return &mSource;
			}
			UIFreetypeSource(Stream* stream) {
				stream->AddRef();
				memset(&mSource, 0, sizeof(FT_StreamRec));
				mSource.descriptor.pointer = reinterpret_cast<void*>(stream);
				mSource.size = static_cast<unsigned long>(stream->GetLength());
				mSource.close = FT_Stream_Close;
				mSource.read = FT_Stream_Read;
				pStream = stream;
			}
		};

		class UICairoContext;

		class UICairoFont : public UIFont
		{
		private:
			friend class UICairoContext;
			cairo_font_face_t* mCairoFace;
			hb_font_t* mHarfbuzzFace;

			FT_Face mFTFace;
			UIFreetypeSource mSource;
			UICairoContext* mContext;
		protected:
			virtual ~UICairoFont();
		public:
			UICairoFont(UICairoContext* pContext, Stream* pStream);
			UIContext* GetContext() const;
		};
	}
}