#include <Skuld/Plugin.h>
#include "VulkanFactory.h"
#include "VulkanContext.h"
#include <WindowInfo.h>

namespace Skuld
{
	namespace Render3D
	{
		Render3DFactory* VulkanFactory::CreateRender3D_VulkanFactory()
		{
			Ptr<VulkanFactory> mRet = new VulkanFactory();

			uint32_t InstanceExtensionCount = 0;

			VkResult mResult;

			mResult = vkEnumerateInstanceExtensionProperties(nullptr, &InstanceExtensionCount, nullptr);

			if (mResult != VK_SUCCESS) return nullptr;

			std::vector<VkExtensionProperties> ExtensionProperties(InstanceExtensionCount);
			mResult = vkEnumerateInstanceExtensionProperties(NULL, &InstanceExtensionCount, ExtensionProperties.data());

			if (mResult != VK_SUCCESS) return nullptr;

			char* EnableExtensionNameArray[2];
			for (uint32_t i = 0; i < InstanceExtensionCount; i++)
			{
				if (strcmp(ExtensionProperties[i].extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0)
					EnableExtensionNameArray[0] = ExtensionProperties[i].extensionName;
				else if (strcmp(ExtensionProperties[i].extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0)
					EnableExtensionNameArray[1] = ExtensionProperties[i].extensionName;
			}

			VkApplicationInfo ApplicationInfo;
			ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			ApplicationInfo.pNext = NULL;
			ApplicationInfo.apiVersion = 0;
			ApplicationInfo.applicationVersion = 1;
			ApplicationInfo.engineVersion = 0;
			ApplicationInfo.pApplicationName = "SkuldEngine";
			ApplicationInfo.pEngineName = "SkuldEngine";

			VkInstanceCreateInfo InstanceCreateInfo;
			InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCreateInfo.pNext = NULL;
			InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;
			InstanceCreateInfo.enabledLayerCount = 0;
			InstanceCreateInfo.enabledExtensionCount = 2;
			InstanceCreateInfo.ppEnabledExtensionNames = EnableExtensionNameArray;

			mResult = vkCreateInstance(&InstanceCreateInfo, nullptr, &mRet->mVkInstance);

			if (mResult != VK_SUCCESS) return nullptr;

			return mRet.Detach();
		}

		VulkanFactory::~VulkanFactory()
		{
			if (mVkInstance != nullptr)
				vkDestroyInstance(mVkInstance, nullptr);
		}

		Render3DContext * VulkanFactory::CreateContext(const WindowInfo* mInfo, const Config* mConfig) const
		{
			Ptr<VulkanContext> mRet = new VulkanContext(this);

			uint32_t mVkQueueFamilyIndex;

			uint32_t PhysicalDeviceCount;

			VkResult mResult = vkEnumeratePhysicalDevices(mVkInstance, &PhysicalDeviceCount, NULL);

			if (mResult != VK_SUCCESS) return nullptr;

			if (PhysicalDeviceCount != 0)
			{
				std::vector<VkPhysicalDevice> PhysicalDevices(PhysicalDeviceCount);

				mResult = vkEnumeratePhysicalDevices(mVkInstance, &PhysicalDeviceCount, PhysicalDevices.data());

				if (mResult != VK_SUCCESS) return nullptr;

				mRet->mVkPhysicalDevice = PhysicalDevices[0];
			}

			VkPhysicalDeviceProperties PhysicalDeviceProperties;
			vkGetPhysicalDeviceProperties(mRet->mVkPhysicalDevice, &PhysicalDeviceProperties);

			uint32_t QueueFamilyCount;

			vkGetPhysicalDeviceQueueFamilyProperties(mRet->mVkPhysicalDevice, &QueueFamilyCount, NULL);

			if (QueueFamilyCount > 0)
			{
				std::vector<VkQueueFamilyProperties> QueueFamilysProperties(QueueFamilyCount);

				vkGetPhysicalDeviceQueueFamilyProperties(mRet->mVkPhysicalDevice, &QueueFamilyCount, QueueFamilysProperties.data());

				for (int i = 0; i < QueueFamilyCount; i++)
				{
					if (QueueFamilysProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						mVkQueueFamilyIndex = i;
						mRet->mVkQueueFamilyProperties = QueueFamilysProperties[i];
						break;
					}
				}
			}


			uint32_t DeviceExtensionCount;

			mResult = vkEnumerateDeviceExtensionProperties(mRet->mVkPhysicalDevice, NULL, &DeviceExtensionCount, NULL);

			if (mResult != VK_SUCCESS) return nullptr;

			std::vector<VkExtensionProperties> DeviceExtensionProperties(DeviceExtensionCount);
			mResult = vkEnumerateDeviceExtensionProperties(mRet->mVkPhysicalDevice, NULL, 
				&DeviceExtensionCount, DeviceExtensionProperties.data());

			if (mResult != VK_SUCCESS) return nullptr;

			char* EnableExtensionNameArray[1];
			for (uint32_t i = 0; i < DeviceExtensionCount; i++)
			{
				if (strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, DeviceExtensionProperties[i].extensionName) == 0)
				{
					EnableExtensionNameArray[0] = DeviceExtensionProperties[i].extensionName;
					break;
				}
			}

			float QueuePriorities[1] = { 0.0 };
			VkDeviceQueueCreateInfo DeviceQueueCreateInfo;
			DeviceQueueCreateInfo.flags = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			DeviceQueueCreateInfo.pNext = NULL;
			DeviceQueueCreateInfo.queueFamilyIndex = mVkQueueFamilyIndex;
			DeviceQueueCreateInfo.queueCount = 1;
			DeviceQueueCreateInfo.pQueuePriorities = QueuePriorities;

			VkDeviceCreateInfo DeviceCreateInfo;
			DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceCreateInfo.pNext = NULL;
			DeviceCreateInfo.queueCreateInfoCount = 1;
			DeviceCreateInfo.pQueueCreateInfos = &DeviceQueueCreateInfo;
			DeviceCreateInfo.enabledLayerCount = 0;
			DeviceCreateInfo.enabledExtensionCount = 1;
			DeviceCreateInfo.ppEnabledExtensionNames = EnableExtensionNameArray;
			DeviceCreateInfo.pEnabledFeatures = NULL;;
			mResult = vkCreateDevice(mRet->mVkPhysicalDevice, &DeviceCreateInfo, NULL, &mRet->mVkDevice);

			if (mResult != VK_SUCCESS) return nullptr;

			return nullptr;
		}
	}
}

#ifdef SKULD_BUILD_DYNAMIC

extern "C" Skuld::CreateRender3DFactoryFunc* GetRender3DFactoryEntry() {
	return Skuld::Render3D::VulkanFactory::CreateRender3D_VulkanFactory;
}

#endif