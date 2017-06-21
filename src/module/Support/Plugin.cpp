#include "PluginEntry.h"
#include <string.h>
#include <Skuld/Exception.h>

namespace Skuld
{
	SkuldEngine* CreateEngine()
	{ 
		SkuldEngine* ret = new (std::nothrow) SkuldEngine();
		memset(ret->static_plugins, 0, sizeof(ret->static_plugins));
		memset(ret->plugins, 0, sizeof(ret->plugins));
		return ret;
	}

	void DestroyEngine(SkuldEngine* engine) 
	{
		if (engine->RefCount() > 1) throw Exception("插件已被锁定");
		engine->Dispose();
	}

	SkuldEngine::~SkuldEngine()
	{
		for (int i = 0;i < sizeof(plugins) / sizeof(DynamicLibrary*);i++)
			if (plugins[i])
				this->plugins[i] ? UnloadPlugin(this, plugins[i]) : UnloadStaticPlugin(this, &static_plugins[i]);
	}

	static inline void* SelectEntry(SkuldEngine* handle, int type, void* entry)
	{
		if (handle->plugins[type] || 
			(handle->static_plugins[type].GetPluginEntry && handle->static_plugins[type].GetPluginSig)) return nullptr;

		PluginType mType = (PluginType)type;
		switch (mType)
		{
		case tRender2D:
			handle->CreateRender2DFactory = (CreateRender2DFactoryFunc*)entry;
			break;
		case tRender3D:
			handle->CreateRender3DFactory = (CreateRender3DFactoryFunc*)entry;
			break;
		case tAudio:
			handle->CreateAudioDevice = (CreateAudioDeviceFunc*)entry;
			break;
		default:
			return nullptr;
		}
		return entry;
	}
	
	const DynamicLibrary* LoadPlugin(SkuldEngine* engine, const String& path)
	{
		if (engine->RefCount() > 1) throw Exception("插件已被锁定");

		Ptr<DynamicLibrary> temp(DynamicLibrary::OpenDynamicLibrary(path));
		if (temp == nullptr) return nullptr;

		std::function<GetPluginEntryFunc> LoadProc = temp->GetSymbol<GetPluginEntryFunc>("Skuld_GetPluginEntry");
		if (LoadProc == nullptr) LoadProc = temp->GetSymbol<GetPluginEntryFunc>("_Skuld_GetPluginEntry");

		std::function<GetPluginSigFunc> SigProc = temp->GetSymbol<GetPluginSigFunc>("Skuld_GetPluginSig");
		if (SigProc == nullptr) SigProc = temp->GetSymbol<GetPluginSigFunc>("_Skuld_GetPluginSig");

		if (LoadProc == nullptr || SigProc == nullptr)
			return nullptr;

		if (!SelectEntry(engine, SigProc(), LoadProc()))
			return nullptr;

		engine->plugins[SigProc()] = temp;
		return temp;
	}

	const StaticPlugin* LoadStaticPlugin(SkuldEngine* engine, const StaticPlugin& plugin)
	{
		if (engine->RefCount() > 1) throw Exception("插件已被锁定");

		int sig = plugin.GetPluginSig();

		if (!SelectEntry(engine, sig, plugin.GetPluginEntry()))
			return nullptr;

		engine->plugins[sig] = 0;
		engine->static_plugins[sig] = plugin;

		return &engine->static_plugins[sig];
	}

	static inline void CleanPlugin(SkuldEngine* engine, int type)
	{
		PluginType mType = (PluginType)type;
		engine->static_plugins[type].GetPluginEntry = nullptr;
		engine->static_plugins[type].GetPluginSig = nullptr;
		engine->plugins[type] = Ptr<DynamicLibrary>(nullptr);

		switch (mType)
		{
		case tRender2D:
			engine->CreateRender2DFactory = nullptr;
			break;
		case tRender3D:
			engine->CreateRender3DFactory = nullptr;
			break;
		case tAudio:
			engine->CreateAudioDevice = nullptr;
			break;
		default:
			break;
		}
	}

	void UnloadStaticPlugin(SkuldEngine* engine, const StaticPlugin* handle)
	{
		if (engine->RefCount() > 1) throw Exception("插件已被锁定");

		StaticPlugin* plugin = (StaticPlugin*)handle;
		int plugin_type = plugin->GetPluginSig();
		CleanPlugin(engine, plugin_type);
	}

	void UnloadPlugin(SkuldEngine* engine, const DynamicLibrary* temp)
	{
		if (engine->RefCount() > 1) throw Exception("插件已被锁定");

		std::function<GetPluginSigFunc> SigProc = temp->GetSymbol<GetPluginSigFunc>("Skuld_GetPluginSig");
		if (SigProc == nullptr) SigProc = temp->GetSymbol<GetPluginSigFunc>("_Skuld_GetPluginSig");

		int plugin_type = SigProc();
		engine->plugins[plugin_type] = nullptr;
		CleanPlugin(engine, plugin_type);
	}
}