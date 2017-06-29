#pragma once
#include "Export.h"
#include "String.h"
#include "DynamicLibrary.h"

namespace Skuld
{
	namespace Render3D
	{
		class Render3DFactory;
	}
	namespace Render2D
	{
		class Render2DFactory;
	}
	namespace Audio
	{
		class AudioFactory;
	}

	typedef Render3D::Render3DFactory* (CreateRender3DFactoryFunc)();
	typedef Render2D::Render2DFactory* (CreateRender2DFactoryFunc)();
	typedef Audio::AudioFactory* (CreateAudioFactoryFunc)();

	class SkuldEngine;
	SKULD_EXPORT SkuldEngine* CreateEngine();
	SKULD_EXPORT void DestroyEngine(SkuldEngine*);

	enum PluginType
	{
		tRender2D,
		tRender3D,
		tAudio
	};

	typedef void* (GetPluginEntryFunc)();
	typedef int (GetPluginSigFunc)();

	struct StaticPlugin
	{
		GetPluginEntryFunc* GetPluginEntry;
		GetPluginSigFunc* GetPluginSig;
	};

	SKULD_EXPORT const DynamicLibrary* LoadPlugin(SkuldEngine*, const String& path);
	SKULD_EXPORT void UnloadPlugin(SkuldEngine*, const DynamicLibrary* handle);

	SKULD_EXPORT const StaticPlugin* LoadStaticPlugin(SkuldEngine* engine, const StaticPlugin& plugin);
	SKULD_EXPORT void UnloadStaticPlugin(SkuldEngine* engine, const StaticPlugin* handle);

	SKULD_EXPORT const void* GetPluginEntry(const SkuldEngine*, PluginType mType);
}