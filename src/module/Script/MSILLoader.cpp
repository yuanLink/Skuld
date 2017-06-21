#include <Skuld/Script/Script.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <vector>

namespace Skuld
{
	namespace Script
	{
		class MSILLoader : public ScriptLoader
		{
		protected:
			~MSILLoader();
		public:
			MonoDomain* mDomain;

			std::vector<MonoImage*> mImages;

			MSILLoader(MonoDomain* mDomain);
			void LoadScript(const uint8_t* data, size_t size);
			void LoadScript(const String& file);
		};

		ScriptLoader* CreateMSILLoader()
		{
			MonoDomain* mDomain = mono_jit_init("MSILLoader");
			return new MSILLoader(mDomain);
		}

		MSILLoader::~MSILLoader()
		{
			for (MonoImage* image : mImages) mono_image_close(image);
			mono_jit_cleanup(mDomain);
		}

		MSILLoader::MSILLoader(MonoDomain * mDomain) : mDomain(mDomain)
		{
		}

		void MSILLoader::LoadScript(const uint8_t* data, size_t size)
		{
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data((char*)data, (uint32_t)size, true, &status);
			mImages.push_back(image);
		}

		void MSILLoader::LoadScript(const String & file)
		{
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open(file.GetStr().c_str(), &status);
			mImages.push_back(image);
		}
	}
}