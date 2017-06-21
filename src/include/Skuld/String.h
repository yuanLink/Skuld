#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include "Export.h"

namespace Skuld
{
#define SKULD_STR(a) U ## a
	class String
	{
	private:
		std::u32string mChars;
		static SKULD_EXPORT std::u32string String_WChar_To_char32_t(const wchar_t* str);
		static SKULD_EXPORT std::wstring String_char32_t_To_WChar(const char32_t* str);
		static SKULD_EXPORT std::u32string String_char16_t_To_char32_t(const char16_t* str);
		static SKULD_EXPORT std::u16string String_char32_t_To_char16_t(const char32_t* str);
		static SKULD_EXPORT std::u32string String_CurrentCP_To_char32_t(const char* str);
		static SKULD_EXPORT std::string String_char32_t_To_CurrentCP(const char32_t* str);
		static SKULD_EXPORT std::u32string String_UTF8_To_char32_t(const char* str);
		static SKULD_EXPORT std::string String_char32_t_To_UTF8(const char32_t* str);
	public:
		static bool IsLetter(char32_t mChar) {
			return (mChar <= SKULD_STR('Z') && mChar >= SKULD_STR('A'))
				|| (mChar <= SKULD_STR('z') && mChar >= SKULD_STR('a'));
		}
		char32_t operator[](size_t mIndex) const {
			return this->mChars[mIndex];
		}
		char32_t& operator[](size_t mIndex) {
			return this->mChars[mIndex];
		}
		const char32_t* GetChars() const { return this->mChars.c_str(); }
		std::string GetStr() const {
			return String_char32_t_To_CurrentCP(this->mChars.c_str());
		}
		std::wstring GetWStr() const {
			return String_char32_t_To_WChar(this->mChars.c_str());
		}
		std::string GetUTF8Str() const {
			return String_char32_t_To_UTF8(this->mChars.c_str());
		}
		std::u16string GetUTF16Str() const {
			return String_char32_t_To_char16_t(this->mChars.c_str());
		}
		std::vector<String> Spilt(const char32_t * const chars, size_t char_count, bool trim = false) const {
			const char32_t * c = this->mChars.c_str();
			uint32_t ptr = 0;
			uint32_t size = 0;
			std::vector<String> ret;
			while (*c)
			{
				bool find = false;
				for (uint32_t i = 0;i < char_count;i++)
				{
					if (chars[i] == *c)
					{
						if (size || !trim) ret.push_back(this->Substr(ptr, size));
						ptr += (size + 1);
						size = 0;
						c++;
						find = true;
						break;
					}
				}
				if (!find)
				{
					c++;
					size++;
				}
			}
			if (size || !trim) ret.push_back(this->Substr(ptr, size));
			return std::move(ret);
		}
		std::vector<String> Spilt(const String& chars, bool trim = false) const {
			return std::move(this->Spilt(chars.mChars.c_str(), chars.CharCount(), trim));
		}
		std::vector<String> Spilt(char32_t charater, bool trim = false) const {
			return std::move(this->Spilt(&charater, 1, trim));
		}

		String Replace(char32_t origin, char32_t replace_char) const {
			size_t size = this->mChars.size();
			String ret;
			ret.mChars.resize(size);
			for (uint32_t i = 0;i < size;i++)
			{
				if (this->mChars[i] == origin) ret.mChars[i] = replace_char;
				else ret.mChars[i] = this->mChars[i];
			}
			return ret;
		}

		String ToUpper() const {
			size_t size = this->mChars.size();
			String ret;
			ret.mChars.resize(size);
			for (size_t i = 0;i < size;i++)
			{
				if (this->mChars[i] >= SKULD_STR('a') && this->mChars[i] <= SKULD_STR('z'))
					ret.mChars[i] = this->mChars[i] + SKULD_STR('A') - SKULD_STR('a');
				else ret.mChars[i] = this->mChars[i];
			}
			return ret;
		}
		String ToLower() const {
			size_t size = this->mChars.size();
			String ret;
			ret.mChars.resize(size);
			for (size_t i = 0;i < size;i++)
			{
				if (this->mChars[i] >= SKULD_STR('A') && this->mChars[i] <= SKULD_STR('Z'))
					ret.mChars[i] = this->mChars[i] + SKULD_STR('a') - SKULD_STR('A');
				else ret.mChars[i] = this->mChars[i];
			}
			return ret;
		}

		size_t CharCount() const {
			return this->mChars.size();
		}

		bool StartWith(const String& str) const {
			return (str.mChars.size() <= this->mChars.size()) ?
				memcmp(str.mChars.c_str(), this->mChars.c_str(), sizeof(char32_t) * str.mChars.size()) == 0 : false;
		}
		bool EndWith(const String& str) const {
			return (str.mChars.size() <= this->mChars.size()) ?
				memcmp(str.mChars.c_str(), &this->mChars[mChars.size() - str.mChars.size()],
					sizeof(char32_t) * mChars.size()) == 0 : false;
		}
		String(const wchar_t * str) {
			this->mChars = String_WChar_To_char32_t(str);
		}
		String(const char32_t * str) {
			this->mChars = str;
		}
		String(const char16_t * str) {
			this->mChars = String_char16_t_To_char32_t(str);
		}
		String(const char* str, bool is_acp = true) {
			if (is_acp) this->mChars = String_CurrentCP_To_char32_t(str);
			else this->mChars = String_UTF8_To_char32_t(str);
		}
		String() : mChars(SKULD_STR("")) {}
		String(const String& str) {
			this->mChars = str.mChars;
		}
		String(String&& str) {
			this->mChars = std::move(str.mChars);
		}
		String& operator=(const String& str) {
			if (this != &str)
				this->mChars = str.mChars;
			return *this;
		}
		String& operator+=(const String& str_a) {
			this->mChars += str_a.mChars;
			return *this;
		}
		String Substr(size_t start, size_t count = -1) const {
			String ret;
			ret.mChars = this->mChars.substr(start, count);
			return ret;
		}
		friend String operator+(const String& str_a, const String& str_b) {
			String ret;
			ret.mChars = str_a.mChars + str_b.mChars;
			return ret;
		}
		friend bool operator==(const String& str_a, const String& str_b) {
			return str_a.mChars == str_b.mChars;
		}
		friend bool operator!=(const String& str_a, const String& str_b) {
			return str_a.mChars != str_b.mChars;
		}
		friend bool operator<(const String& str_a, const String& str_b) {
			return str_a.mChars < str_b.mChars;
		}
		friend bool operator>(const String& str_a, const String& str_b) {
			return str_a.mChars > str_b.mChars;
		}
		friend bool operator<=(const String& str_a, const String& str_b) {
			return str_a.mChars <= str_b.mChars;
		}
		friend bool operator>=(const String& str_a, const String& str_b) {
			return str_a.mChars >= str_b.mChars;
		}
	};

	static inline std::wostream& operator<<(std::wostream& stream, const String& str)
	{
		stream << str.GetWStr();
		return stream;
	}

	static inline std::wistream& operator >> (std::wistream& stream, String& str)
	{
		std::wstring _str;
		stream >> _str;
		str = _str.c_str();
		return stream;
	}

	static inline std::ostream& operator<<(std::ostream& stream, const String& str)
	{
		stream << str.GetStr();
		return stream;
	}

	static inline std::istream& operator >> (std::istream& stream, String& str)
	{
		std::string _str;
		stream >> _str;
		str = _str.c_str();
		return stream;
	}

#ifdef _MSC_VER
	class SKULD_EXPORT String;
#endif
}