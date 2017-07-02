#include <Skuld/Ptr.hpp>
#include <Skuld/Plugin.h>
#include <Skuld/Core/Engine3D.h>
#include <Skuld/Render3D/Render3DContext.h>
#include <Skuld/Render3D/Render3DFactory.h>
#include <Skuld/Codec/BitmapCodec.h>
#include <Skuld/FileStream.h>

using namespace Skuld::UI;
using namespace Skuld::Core;
using namespace Skuld;
using namespace Skuld:: Render3D;

#if defined(_DEBUG)
#define CONF "Debug"
#else
#define CONF "Release"
#endif

const float4 mPositions[] = {
	float4(0, 0, 0, 1),
	float4(1, 0, 0, 1),
	float4(0, 1, 0, 1),
	float4(0, 0, 1, 1),
	float4(1, 0, 1, 1),
	float4(1, 1, 0, 1),
	float4(0, 1, 1, 1),
	float4(1, 1, 1, 1),
};

const int mIndices[] = {
	0, 1, 2
};

const size_t mStrides[] = {
	sizeof(mPositions[0])
};

int main()
{
	SkuldEngine* mSkuldEngine = CreateEngine();
	const DynamicLibrary* mPlugin = LoadPlugin(mSkuldEngine, SKULD_BUILD_DIR "/" CONF "/plugin/Render3D_D3D11.dll");

	{
		Ptr<Displayable> mDis = Displayable::Create(300, 300);

		Ptr<Effect> mEffect = Effect::CompileFromFile(
			SKULD_BUILD_DIR "/../../resources/HLSL.hlsl",
			SKULD_BUILD_DIR "/../../resources/EffectScript.json",
			Extra::ShaderBinaryFormat_DXBC
		);

		Ptr<Bitmap> mBitmap = Codec::DecodeBitmapFromStream(
			MakePtr(FileStream::Open(SKULD_BUILD_DIR "/../../resources/test.tga",
				Open))
		);

		std::function<CreateRender3DFactoryFunc> mEntry = (CreateRender3DFactoryFunc*)GetPluginEntry(mSkuldEngine, tRender3D);

		Ptr<Render3DFactory> mFactory = mEntry();
		Ptr<Render3DContext> mContext = mFactory->CreateContext(mDis->GetWindowInfo(), GetGlobalConfig());

		String mVSName = mEffect->GetVertexShader(0);
		String mPSName = mEffect->GetPixelShader(0);
		Ptr<ShaderObject> mVS = mContext->CreateShader(mEffect->GetShader(mVSName),
			mEffect->GetShaderSize(mVSName), ShaderType_VertexShader);
		Ptr<ShaderObject> mPS = mContext->CreateShader(mEffect->GetShader(mPSName),
			mEffect->GetShaderSize(mPSName), ShaderType_PixelShader);
		Ptr<InputLayout> mLayout = mContext->CreateInputLayout(mEffect->GetInputLayoutAttri(0),
			mEffect->GetInputLayoutAttriCount(0), mVS);

		Ptr<BufferObject> mVertexBuffer[] = {
			mContext->CreateBufferObject(
				mPositions,
				sizeof(mPositions),
				Access_GPURead | Access_GPUWrite,
				BufferBind_VertexBuffer
			),
		};

		Ptr<BufferObject> mIndexBuffer = mContext->CreateBufferObject(mIndices, sizeof(mIndices), 
			Access_GPURead | Access_GPUWrite, BufferBind_IndexBuffer);

		Ptr<Texture> mTexture2D = mContext->CreateTexture2D(mBitmap->GetPixels(), mBitmap->GetWidth(),
			mBitmap->GetHeight(), mBitmap->GetPixelFormat(), Access_GPURead | Access_GPUWrite, TextureBind_ShaderResource);

		mContext->SetVertexBuffer(&mVertexBuffer[0], mStrides, sizeof(mVertexBuffer) / sizeof(Ptr<BufferObject>));
		mContext->SetIndexBuffer(mIndexBuffer, sizeof(mIndices[0]));
		mContext->SetShader(ShaderType_VertexShader, mVS);
		mContext->SetShader(ShaderType_PixelShader, mPS);
		mContext->SetInputLayout(mLayout);

		float4 mCameraBuffer[16];

		Ptr<BufferObject> mCamera = mContext->CreateBufferObject(
			mCameraBuffer, sizeof(mCameraBuffer), Access_GPURead | Access_GPUWrite, BufferBind_ConstantBuffer);

		while (!mDis->MainLoop())
		{
			mCamera->Update(mCameraBuffer, sizeof(mCameraBuffer));
			mContext->DoRender();
		}
	}

	UnloadPlugin(mSkuldEngine, mPlugin);
	DestroyEngine(mSkuldEngine);
	return 0;
}