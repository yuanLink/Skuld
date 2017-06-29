#pragma once
#include <Skuld/Plugin.h>
#include <Skuld/IDisposable.h>
#include <functional>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	class SkuldEngine : public IDisposable
	{
	public:
		virtual ~SkuldEngine();
		Ptr<DynamicLibrary> plugins[3];
		StaticPlugin static_plugins[3];

		void* mEntry[3];

		std::function<CreateRender3DFactoryFunc> CreateRender3DFactory;
		std::function<CreateAudioFactoryFunc> CreateAudioFactory;
		std::function<CreateRender2DFactoryFunc> CreateRender2DFactory;
	};
}