#pragma once
#include "Render3DFactory.h"
#include "ShaderObject.h"
#include "InputLayout.h"
#include "Texture.h"
#include "BufferObject.h"
#include "RenderState.h"
#include "../PixelFormat.h"

namespace Skuld
{
	namespace Render3D
	{
		enum AccessFlag
		{
			Access_CPUWrite = 1 << 0,
			Access_CPURead = 1 << 1,
			Access_GPUWrite = 1 << 2,
			Access_GPURead = 1 << 3,
		};

		static inline AccessFlag operator|(AccessFlag a, AccessFlag b)
		{
			return (AccessFlag)((int)a | (int)b);
		}

		static inline AccessFlag operator&(AccessFlag a, AccessFlag b)
		{
			return (AccessFlag)((int)a & (int)b);
		}

		enum BufferBindFlag
		{
			BufferBind_VertexBuffer = 1 << 0,
			BufferBind_IndexBuffer = 1 << 1,
			BufferBind_ConstantBuffer = 1 << 2,
			BufferBind_ShaderResource = 1 << 3,
		};

		static inline BufferBindFlag operator|(BufferBindFlag a, BufferBindFlag b)
		{
			return (BufferBindFlag)((int)a | (int)b);
		}

		static inline BufferBindFlag operator&(BufferBindFlag a, BufferBindFlag b)
		{
			return (BufferBindFlag)((int)a & (int)b);
		}

		enum TextureBindFlag
		{
			TextureBind_ShaderResource = 1 << 3,
		};

		static inline TextureBindFlag operator|(TextureBindFlag a, TextureBindFlag b)
		{
			return (TextureBindFlag)((int)a | (int)b);
		}

		static inline TextureBindFlag operator&(TextureBindFlag a, TextureBindFlag b)
		{
			return (TextureBindFlag)((int)a & (int)b);
		}

		class Render3DContext : public IDisposable
		{
		protected:
			virtual ~Render3DContext() {}
		public:
			virtual void SetVertexBuffer(BufferObject** mBufferObject, const size_t* mStrides, size_t mCount) = 0;
			virtual void SetShader(ShaderType mType, ShaderObject* mShader) = 0;
			virtual void SetInputLayout(InputLayout* mInputLayout) = 0;
			virtual void SetIndexBuffer(BufferObject* mBufferObject, size_t mStride) = 0;

			virtual const Render3DFactory* GetFactory() const = 0;

			virtual void DoRender() = 0;

			virtual BufferObject* CreateBufferObject(const void* mData, size_t mBufferSize,
				AccessFlag mAccess, BufferBindFlag mBind) = 0;
			virtual Texture* CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth,
				PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind) = 0;
			virtual Texture* CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind) = 0;
			virtual Texture* CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth,
				uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat, AccessFlag mAccess, TextureBindFlag mBind) = 0;
			virtual ShaderObject* CreateShader(const uint8_t* mCode, size_t mCodeSize, ShaderType mType) = 0;
			virtual InputLayout* CreateInputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize, ShaderObject* mShader) = 0;
		};
	}
}