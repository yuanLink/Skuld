#pragma once
#include "Export.h"
#include "String.h"

namespace Skuld
{
	class Config
	{
	public:
		virtual bool GetWindowed() const = 0;
		virtual const String& GetDefaultFontPath() const = 0;
	};

	SKULD_EXPORT const Config* GetGlobalConfig();

	SKULD_EXPORT void SaveGlobalConfig(const String& path);
	SKULD_EXPORT void LoadGlobalConfigFromFile(const String& path);
	SKULD_EXPORT void LoadGlobalConfigFromString(const String& path);

	SKULD_EXPORT void SetConfigFromProgramOptionsA(int argc, char** argv, const String& caption);
	SKULD_EXPORT void SetConfigFromProgramOptionsW(int argc, wchar_t** argv, const String& caption);
}