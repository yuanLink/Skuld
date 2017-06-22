#pragma once
#include "String.h"
#include "String.h"

namespace Skuld
{
	String NormalizePath(const String& path);
	String GetRelativePath(const String& path, const String& base_path);
	String GetRelativePath(const String& path);
	String GetParentDirectory(const String & path);
	String GetFullPath(const String& path);
	String GetFullPath(const String& path, const String& base_path);
	String GetCurrentDir();
	String GetHomePath();
	bool IsFullPath(const String& path);
	String GetFileName(const String& path);
}