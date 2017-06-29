#include <Skuld/LanguagePack.h>
#include <json/json.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/Exception.h>
#include <map>

namespace Skuld
{
	class LanguagePackImpl : public LanguagePack
	{
	protected:
		virtual ~LanguagePackImpl() {};

	public:
		std::map<String, std::map<String, String> > mDict;
		std::map<String, String>* mCurrentDict;
		virtual void SetLang(const String& mLang);

		virtual const String& GetString(const String& mKey) const;
	};

	static bool ParseJSON(std::map<String, std::map<String, String> >& mDict, const String& mJSON)
	{
		Json::Value mRoot;
		Json::Reader mReader;
		if (!mReader.parse(mJSON.GetUTF8Str(), mRoot, false))
			return false;

		std::vector<std::string>&& mLangs = mRoot.getMemberNames();
		for (std::string& mLang : mLangs)
		{
			Json::Value& mLangDict = mRoot[mLang];
			std::vector<std::string>&& mKeys = mLangDict.getMemberNames();

			for (std::string& mKey : mKeys)
			{
				Json::Value& mWord = mLangDict[mKey];

				if (!mWord.isString())
					return false;
				String mLangTemp(mLang.c_str(), false);
				String mKeyTemp(mKey.c_str(), false);
				String mWordTemp(mWord.asCString(), false);

				std::map<String, String>& mDestDict = mDict[mLangTemp];

				if (mDestDict.find(mKeyTemp) != mDestDict.end())
					return false;

				mDestDict.emplace(mKeyTemp, mWordTemp);
			}
		}
		return true;
	}

	LanguagePack * LanguagePack::LoadFromJSON(const String & mJSON)
	{
		Ptr<LanguagePackImpl> mRet = new LanguagePackImpl();

		if (!ParseJSON(mRet->mDict, mJSON))
			return nullptr;

		return mRet.Detach();
	}

	LanguagePack * LanguagePack::LoadFromJSONs(const String * mJSONs, size_t mCount)
	{
		Ptr<LanguagePackImpl> mRet = new LanguagePackImpl();

		for (size_t i = 0; i < mCount; i++)
			if (!ParseJSON(mRet->mDict, mJSONs[i]))
				return nullptr;

		return mRet.Detach();
	}

	void LanguagePackImpl::SetLang(const String & mLang)
	{
		std::map<String, std::map<String, String> >::iterator it = mDict.find(mLang);
		if (it == mDict.end()) throw Exception("Unavaliable lang");
		mCurrentDict = &it->second;
	}

	const String & LanguagePackImpl::GetString(const String & mKey) const
	{
		try
		{
			return mCurrentDict->at(mKey);
		}
		catch (std::out_of_range& e)
		{
			throw Exception(e.what());
		}
	}
}