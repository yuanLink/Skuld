#include "VulkanContext.h"

namespace Skuld
{
	namespace Render3D
	{
		VulkanContext::~VulkanContext()
		{
			if (this->mVkDevice)
				vkDestroyDevice(this->mVkDevice, nullptr);
		}










		void VulkanContext::SetVertexBuffer(BufferObject ** mBufferObject, const size_t * mStrides, size_t mCount)
		{
		}
		void VulkanContext::SetShader(ShaderType mType, ShaderObject * mShader)
		{
		}
		void VulkanContext::SetInputLayout(InputLayout * mInputLayout)
		{
		}
		void VulkanContext::SetIndexBuffer(BufferObject * mBufferObject, size_t mStride)
		{
		}
		const Render3DFactory * VulkanContext::GetFactory() const
		{
			return nullptr;
		}
		void VulkanContext::DoRender()
		{
		}
		BufferObject * VulkanContext::CreateBufferObject(const void * mData, size_t mBufferSize, AccessFlag mAccess, BufferBindFlag mBind)
		{
			return nullptr;
		}
		Texture * VulkanContext::CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			return nullptr;
		}
		Texture * VulkanContext::CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			return nullptr;
		}
		Texture * VulkanContext::CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind)
		{
			return nullptr;
		}
		ShaderObject * VulkanContext::CreateShader(const uint8_t * mCode, size_t mCodeSize, ShaderType mType)
		{
			return nullptr;
		}
		InputLayout * VulkanContext::CreateInputLayout(const ShaderInputLayoutAttri * mAttri, size_t mSize, ShaderObject * mShader)
		{
			return nullptr;
		}
	}
}