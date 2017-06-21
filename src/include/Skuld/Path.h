#pragma once
#include "String.h"
#include "Export.h"

namespace Skuld
{
	class SKULD_EXPORT Path final
	{
	private:
		Path();
		Path(const Path&&);
	public:
		static String NormalizePath(const String& path);
		static String GetRelativePath(const String& path, const String& base_path);
		static String GetRelativePath(const String& path);
		static String GetParentDirectory(const String & path);
		static String GetFullPath(const String& path);
		static String GetFullPath(const String& path, const String& base_path);
		static String GetCurrentDir();
		static String GetHomePath();
		static bool IsFullPath(const String& path);
		static String GetFileName(const String& path);
	};
}