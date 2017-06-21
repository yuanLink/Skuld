#include "UICairoFont.h"
#include <Skuld/Exception.h>
#include "UICairoContext.h"

namespace Skuld
{
	namespace UI
	{
		class FTLibrary
		{
		public:
			FT_Library mLibrary = nullptr;
			FTLibrary() {
				FT_Error err;
				err = FT_Init_FreeType(&mLibrary);
				if (err)
					throw Exception("Freetype 初始化失败");
			}
			~FTLibrary() {
				FT_Done_FreeType(mLibrary);
			}
		};

		static FTLibrary mFT;

		UICairoFont::UICairoFont(UICairoContext* pContext, Stream* stream) : mSource(stream), mContext(pContext)
		{
			FT_Error err;

			FT_Open_Args args;
			memset(&args, 0, sizeof(args));
			args.flags = FT_OPEN_STREAM;
			args.stream = this->mSource.FTSource();
			err = FT_Open_Face(mFT.mLibrary, &args, 0, &this->mFTFace);

			this->mCairoFace = cairo_ft_font_face_create_for_ft_face(this->mFTFace, 0);
			this->mHarfbuzzFace = hb_ft_font_create_referenced(this->mFTFace);
		}

		UICairoFont::~UICairoFont()
		{
			hb_font_destroy(mHarfbuzzFace);
			cairo_font_face_destroy(this->mCairoFace);
			FT_Done_Face(this->mFTFace);
		}

		UIContext* UICairoFont::GetContext() const { return this->mContext; }
	}
}