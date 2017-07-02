#pragma once

#include <Skuld/Render3D/Render3DContext.h>
#include "VulkanFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		class VulkanContext : public Render3DContext
		{
		protected:
			virtual ~VulkanContext();

			VkPhysicalDevice mVkPhysicalDevice = nullptr;
			VkDevice mVkDevice = nullptr;
			VkQueueFamilyProperties mVkQueueFamilyProperties;

			const VulkanFactory* mFactory;

			friend class VulkanFactory;
			VulkanContext(const VulkanFactory* mFactory) : mFactory(mFactory) {}
		public:

			virtual void SetVertexBuffer(BufferObject** mBufferObject, const size_t* mStrides, size_t mCount);
			virtual void SetShader(ShaderType mType, ShaderObject* mShader);
			virtual void SetInputLayout(InputLayout* mInputLayout);
			virtual void SetIndexBuffer(BufferObject* mBufferObject, size_t mStride);

			virtual const Render3DFactory* GetFactory() const;

			virtual void DoRender();

			virtual BufferObject* CreateBufferObject(const void* mData, size_t mBufferSize,
				AccessFlag mAccess, BufferBindFlag mBind);
			virtual Texture* CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth,
				PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);
			virtual Texture* CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);
			virtual Texture* CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind);
			virtual ShaderObject* CreateShader(const uint8_t* mCode, size_t mCodeSize, ShaderType mType);
			virtual InputLayout* CreateInputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize, ShaderObject* mShader);
		};
	}
}