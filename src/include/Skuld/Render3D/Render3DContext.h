#pragma once
#include "Render3DFactory.h"
#include "Shader.h"
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

		};

		enum BindFlag
		{

		};

		class Render3DContext : public IDisposable
		{
		protected:
			virtual ~Render3DContext() {}
		public:
			virtual Texture* CreateTexture1D(const uint8_t * mPixels, uint32_t mWidth, PixelFormat mPixelFormat) = 0;
			virtual Texture* CreateTexture2D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, PixelFormat mPixelFormat) = 0;
			virtual Texture* CreateTexture3D(const uint8_t * mPixels, uint32_t mWidth, uint32_t mHeight, uint32_t mDepth, PixelFormat mPixelFormat) = 0;
			virtual const Render3DFactory* GetFactory() const = 0;
			virtual Shader* CreateShader(const uint8_t* mCode, size_t mCodeSize, ShaderType mType) = 0;
			virtual InputLayout* CreateInputLayout(const ShaderInputLayoutAttri* mAttri, size_t mSize, Shader* mShader) = 0;
		};
	}
}