#pragma once

#include <Skuld/Render3D/Render3DFactory.h>
#include <Skuld/Ptr.hpp>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace Skuld
{
	namespace Render3D
	{
		class VulkanContext;
		class VulkanFactory : public Render3DFactory
		{
		protected:
			virtual ~VulkanFactory();
			VkInstance mVkInstance = nullptr;
		public:
			Render3DContext* CreateContext(const WindowInfo* mInfo, const Config* mConfig) const;
			CheckFlags GetCheckFlag() const { return AvaliableRender3DAPI_Vulkan; }

			static Render3DFactory* CreateRender3D_VulkanFactory();
		};
	}
}