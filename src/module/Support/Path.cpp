#include <Skuld/Path.h>
#include <memory>
#include <stack>
#ifdef _MSC_VER
#include <direct.h>
#include <shlobj.h>
#define tgetcwd _getcwd
#else
#include <unistd.h>
#define tgetcwd getcwd
#include <stdlib.h>
#endif

namespace Skuld
{
#ifdef _WIN32
	static const char* path_spilt = "\\";
#else
	static const char* path_spilt = "/";
#endif
	String GetRelativePathInternal(const String & path, const String& base_path)
	{
		String ret = GetFullPath(path);

		std::vector<String> from_tokens = ret.Spilt(SKULD_STR("\\/"), 2, true);
		std::vector<String> to_tokens = base_path.Spilt(SKULD_STR("\\/"), 2, true);

		size_t _i = 0;
		while (_i < from_tokens.size() && _i < to_tokens.size() && from_tokens[_i] == to_tokens[_i]) _i++;
		ret = "";
		for (size_t i = 0;i < _i;i++) ret += (String("..") + path_spilt);
		for (size_t i = _i;i < from_tokens.size();i++) ret += (from_tokens[i] + path_spilt);

		return ret;
	}

	String GetRelativePath(const String& path, const String& base_path)
	{
		return GetRelativePathInternal(path, GetFullPath(base_path));
	}

	String GetRelativePath(const String & path)
	{
		return GetRelativePathInternal(path, GetCurrentDir());
	}

	String NormalizePath(const String & path)
	{
		std::vector<String> path_tokens = path.Spilt(SKULD_STR("\\/"), 2, true);
		String ret;
		std::stack<String*> strs;
		for (size_t i = 0;i < path_tokens.size();i++)
		{
			if (path_tokens[i] == ".") continue;
			else if (path_tokens[i] == "..") strs.pop();
			else strs.push(&path_tokens[i]);
		}

		while (!strs.empty())
		{
			ret = (*strs.top()) + path_spilt + ret;
			strs.pop();
		}
		return ret;
	}

	String GetParentDirectory(const String& path)
	{
		String temp = GetFullPath(path);
		std::vector<String> ptr = temp.Spilt(SKULD_STR("\\/"), 2, true);
		String ret = "";
		for (size_t i = 0;i < ptr.size() - 1;i++) ret = ret + (ptr[i] + path_spilt);
		return ret;
	}

	static String GetFullPathInternal(const String & path, const String & base_path)
	{
		if (IsFullPath(path))
		{
			return NormalizePath(path);
		}
		else
		{
#ifndef _WIN32
			if (path.StartWith("~/"))
			{
				return NormalizePath(GetHomePath() + path.Substr(1, path.CharCount() - 1));
			}
#endif
			std::vector<String> path_tokens = path.Spilt(SKULD_STR("\\/"), 2, true),
				base_tokens = base_path.Spilt(SKULD_STR("\\/"), 2, true);

			std::stack<const String*> rets;
			for (uint32_t i = 0;i < base_tokens.size();i++) rets.push(&base_tokens[i]);

			for (uint32_t i = 0;i < path_tokens.size();i++)
			{
				if (path_tokens[i] == ".") continue;
				else if (path_tokens[i] == "..") rets.pop();
				else rets.push(&path_tokens[i]);
			}

			String ret = "";
			while (!rets.empty())
			{
				ret = (*rets.top()) + path_spilt + ret;
				rets.pop();
			}

#ifdef _WIN32
			return ret;
#else
			return path_spilt + ret;
#endif
		}
	}

	String GetFullPath(const String& path, const String& base_path)
	{
		return GetFullPathInternal(path, GetFullPath(base_path));
	}

	String GetFullPath(const String & path)
	{
		return GetFullPathInternal(path, GetCurrentDir());
	}

	String GetCurrentDir()
	{
		return std::unique_ptr<char[]>(tgetcwd(nullptr, 0)).get();
	}

	String GetHomePath()
	{
#ifdef _WIN32
		wchar_t path[MAX_PATH + 2];
		SHGetSpecialFolderPathW(nullptr, path, CSIDL_PERSONAL, FALSE);
		return path;
#elif defined(__MACOSX__)
		return "/User/";
#elif defined(__ANDROID__)
		return "/mnt/sdcard/";
#else
		return getenv("HOME");
#endif
	}

	bool IsFullPath(const String & path)
	{
#ifdef _WIN32
		return path[1] == ':' && String::IsLetter(path[0]);
#else
		static const char* root_dir[] = {
			"/home/", "/boot/", "/etc/",
			"/bin/",  "/cdrom/", "/dev/",
			"/lib/", "/lib64/", "/media/",
			"/mnt/", "/opt/", "/proc/", "/root/",
			"/run/", "/sbin/", "/snap/", "/srv/",
			"/sys/", "/tmp/", "/usr/", "/var/"
		};
		for (uint32_t i = 0;i < sizeof(root_dir) / sizeof(char*);i++)
		{
			if (path.StartWith(root_dir[i]))
				return true;
		}
		return false;
#endif
	}

	String GetFileName(const String & path)
	{
		std::vector<String> token = path.Spilt(SKULD_STR("/\\"), 2, true);
		String ret = token[token.size() - 1];

		for (size_t i = 1;i < ret.CharCount();i++)
			if (ret[i] == '.')
				return ret.Substr(0, i);
		return ret;
	}
}