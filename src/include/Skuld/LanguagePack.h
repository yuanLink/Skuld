#pragma once

#include "Stream.h"
#include "IDisposable.h"

namespace Skuld
{
	class LanguagePack : public IDisposable
	{
	protected:
		virtual ~LanguagePack() {}
	public:
		virtual const String& GetString(const String& mKey) const = 0;
		virtual void SetLang(const String& mLang) = 0;

		static SKULD_EXPORT LanguagePack* LoadFromJSON(const String& mJSON);
		static SKULD_EXPORT LanguagePack* LoadFromJSONs(const String* mJSONs, size_t mCount);
	};
}