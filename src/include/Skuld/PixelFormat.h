#pragma once

#include "Export.h"

namespace Skuld
{
	enum PixelFormat
	{
		PixelFormat_RGBA_8888, //RRRRRRRR GGGGGGGG BBBBBBBB AAAAAAAA
		PixelFormat_BGRA_8888, //BBBBBBBB GGGGGGGG RRRRRRRR AAAAAAAA
		PixelFormat_RGB_888,   //RRRRRRRR GGGGGGGG BBBBBBBB
		PixelFormat_BGR_888,   //BBBBBBBB GGGGGGGG RRRRRRRR
		PixelFormat_RGB_565,   //GGGBBBBB RRRRRGGG
		PixelFormat_ARGB_1555, //GGGBBBBB ARRRRRGG
	};

	SKULD_EXPORT uint32_t PixelFormatDepth(PixelFormat format);
}