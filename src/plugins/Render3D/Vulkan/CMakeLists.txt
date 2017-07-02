find_package(Vulkan)

if (Vulkan_FOUND)
	include_directories(${Vulkan_INCLUDE_DIR})

	add_plugin(Render3D Vulkan
		VulkanContext.cpp
		VulkanContext.h
		VulkanFactory.cpp
		VulkanFactory.h)

	target_link_libraries(Render3D_Vulkan ${Vulkan_LIBRARY})
endif()