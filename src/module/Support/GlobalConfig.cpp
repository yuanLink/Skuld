#include <Skuld/GlobalConfig.h>
#include <tinyxml2.h>
#include <boost/program_options.hpp>
#include <string>
#ifdef _WIN32
#include <io.h>
#define access_ _access
#else
#include <unistd.h>
#define access_ access
#endif
using namespace boost::program_options;

namespace Skuld
{
	static inline String getSystemFont()
	{
		return "./resource/default.ttf";
	}

	template<typename T>
	static inline tinyxml2::XMLElement* insert_conf(tinyxml2::XMLNode* root, const char* type, T value)
	{
		tinyxml2::XMLElement* eNode = root->GetDocument()->NewElement("conf");
		eNode->SetAttribute("type", type);
		eNode->SetText(value);
		root->InsertEndChild(eNode);
		return eNode;
	}

	class GlobalConfig : public Config
	{
	public:
		String mDefaultFontPath;
		bool mWindowed = true;

		void init(tinyxml2::XMLDocument& document)
		{
			tinyxml2::XMLNode* root = document.FirstChildElement("SkuldEngine");
			tinyxml2::XMLElement* nodes = root->FirstChildElement("conf");
			while (nodes)
			{
				const char* _attri = nodes->Attribute("type");
				if (_attri != nullptr)
				{
					std::string attri = _attri;

					if (attri == "DefaultFontPath")
					{
						const char* df = nodes->GetText();
						if (access_(df, 0) != -1)
							mDefaultFontPath = df;
						else mDefaultFontPath = getSystemFont();
					}
				}

				nodes = nodes->NextSiblingElement("conf");
			}
		}

		GlobalConfig(const String& conf_path) {
			tinyxml2::XMLDocument document;
			bool succeed = false;
			succeed = document.LoadFile(conf_path.GetStr().c_str()) == tinyxml2::XML_SUCCESS;
			if (succeed) init(document);
			else
			{
				mDefaultFontPath = getSystemFont();
			}
		}

		GlobalConfig(const char* xml) {
			tinyxml2::XMLDocument document;
			bool succeed = false;
			succeed = document.Parse(xml) == tinyxml2::XML_SUCCESS;
			if (succeed) init(document);
			else
			{
				mDefaultFontPath = getSystemFont();
			}
		}

		GlobalConfig() {
			mDefaultFontPath = getSystemFont();
		}

		void Save(const String& path) {
			tinyxml2::XMLDocument doc;
			doc.InsertFirstChild(doc.NewDeclaration());
			tinyxml2::XMLElement* root = doc.NewElement("SkuldEngine");
			doc.InsertEndChild(root);

			insert_conf(root, "DefaultFontPath", mDefaultFontPath.GetStr().c_str());

			doc.SaveFile(path.GetStr().c_str());
		}

		bool GetWindowed() const {
			return this->mWindowed;
		}

		const String& GetDefaultFontPath() const { return this->mDefaultFontPath; }
	};

	static GlobalConfig mConfig;
	void SaveGlobalConfig(const String& path) { mConfig.Save(path); }
	void LoadGlobalConfigFromFile(const String& path) { mConfig = GlobalConfig(path); }
	void LoadGlobalConfigFromString(const String& xml) { mConfig = GlobalConfig(xml.GetStr().c_str()); }

	static void SetConfigFromVariablesMap(const variables_map& mVM, const options_description& mDesc)
	{
		if (mVM.count("help")) std::cout << mDesc << std::endl;
		else
		{
			if (mVM.count("windowed")) mConfig.mWindowed = true;
		}
	}

	void SetConfigFromProgramOptionsA(int argc, char** argv, const String& caption)
	{
		options_description mDesc(caption.GetStr());
		mDesc.add_options()
			("help,h", "Show help.")
			("windowed", "Weather run game in a window.");
		variables_map mVM;
		store(command_line_parser(argc, argv).options(mDesc).run(), mVM);
		SetConfigFromVariablesMap(mVM, mDesc);
	}

	void SetConfigFromProgramOptionsW(int argc, wchar_t** argv, const String& caption)
	{
		options_description mDesc(caption.GetStr());
		mDesc.add_options()
			("help,h", "Show help.")
			("windowed", "Weather run game in a window.");
		variables_map mVM;
		store(wcommand_line_parser(argc, argv).options(mDesc).run(), mVM);
		SetConfigFromVariablesMap(mVM, mDesc);
	}

	const Config* GetGlobalConfig() { return &mConfig; }
}