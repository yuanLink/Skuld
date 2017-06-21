#include <Skuld/License.h>
#include "License/cairo_license_mpl.h"
#include "License/cpufeature_license.h"
#include "License/DDSTextureLoader_license.h"
#include "License/etc1_license.h"
#include "License/freetype_license.h"
#include "License/libpng_license.h"
#include "License/lua_license.h"
#include "License/mono_license.h"
#include "License/pixman_license.h"
#include "License/tinyxml2_license.h"
#include "License/zlib_license.h"
#include "License/liblzma_license.h"
#include "License/harfbuzz_license.h"
#include "License/boost_license.h"
#include "License/jsoncpp_license.h"
#include "License/jpeg_license.h"

namespace Skuld
{
	const char* GetThirdPartyLicense(const String& str)
	{
		String lower = str.ToLower();

		if (lower == "cpufeature") return (const char*)cpufeature_license;
		else if (lower == "DDSTextureLoader") return (const char*)DDSTextureLoader_license;
		else if (lower == "etc1") return (const char*)etc1_license;
		else if (lower == "freetype") return (const char*)freetype_license;
		else if (lower == "libpng") return (const char*)libpng_license;
		else if (lower == "lua") return (const char*)lua_license;
		else if (lower == "pixman") return (const char*)pixman_license;
		else if (lower == "tinyxml2") return (const char*)tinyxml2_license;
		else if (lower == "zlib") return (const char*)zlib_license;
		else if (lower == "cairo") return (const char*)cairo_license;
		else if (lower == "mono") return (const char*)mono_license;
		else if (lower == "liblzma") return (const char*)liblzma_license;
		else if (lower == "harfbuzz") return (const char*)harfbuzz_license;
		else if (lower == "boost") return (const char*)boost_license;
		else if (lower == "jsoncpp") return (const char*)jsoncpp_license;
		else if (lower == "jpeg") return (const char*)jpeg_license;

		return nullptr;
	}
}