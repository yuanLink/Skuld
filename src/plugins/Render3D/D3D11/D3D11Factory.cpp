#include <Skuld/Plugin.h>
#include "D3D11Factory.h"
#include "D3D11Context.h"
#include <WindowInfo.h>

#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		static Render3DFactory* CreateRender3D_D3D11Factory()
		{
			return new (std::nothrow) D3D11Factory();
		}

		Render3DContext * D3D11Factory::CreateContext(const WindowInfo* mInfo, const Config* mConfig) const
		{
			uint32_t msaa_quality = 0;

			const D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_SOFTWARE
			};
			const uint32_t numDriverTypes = ARRAYSIZE(driverTypes);
			const D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};
			const uint32_t numFeatureLevels = ARRAYSIZE(featureLevels);

			D3D_DRIVER_TYPE mDriverType;
			D3D_FEATURE_LEVEL mFeatureLevel;
			CComPtr<ID3D11Device> mDevice;
			CComPtr<ID3D11DeviceContext> mContext;
			CComPtr<IDXGISwapChain> mSwapChain;

			RECT mRect;
			GetWindowRect(mInfo->hWnd, &mRect);

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			memset(&swapChainDesc, 0, sizeof(swapChainDesc));
			swapChainDesc.BufferCount = 1;
			swapChainDesc.BufferDesc.Width = mRect.right - mRect.left;
			swapChainDesc.BufferDesc.Height = mRect.bottom - mRect.top;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = mInfo->hWnd;
			swapChainDesc.Windowed = mConfig->GetWindowed();
			swapChainDesc.SampleDesc.Count = msaa_quality < 1 ? 1 : 4;
			swapChainDesc.SampleDesc.Quality = msaa_quality < 1 ? 0 : msaa_quality - 1;
			HRESULT hr = S_OK;
			uint32_t createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				mDriverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
					D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain,
					&mDevice, &mFeatureLevel, &mContext);
				if (SUCCEEDED(hr))
					break;
			}

			CComPtr<ID3D11Texture2D> mColorBuffer;
			hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&mColorBuffer));
			RETURN_NULL_IF_FAILED(hr);

			Ptr<D3D11Context> mRet = new D3D11Context(this, mDriverType, mFeatureLevel,
				mDevice, mContext, mSwapChain);

			mRet->mFBO = D3D11FrameBufferObject::Create(mRet, mColorBuffer, msaa_quality);

			return mRet.Detach();
		}
	}
}

extern "C" Skuld::CreateRender3DFactoryFunc* GetRender3DFactoryEntry() { 
	return Skuld::Render3D::CreateRender3D_D3D11Factory;
}