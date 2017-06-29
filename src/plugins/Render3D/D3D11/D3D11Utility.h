#include "D3D11Context.h"
#include <vector>
#include <tuple>
#include <memory>

#define RETURN_NULL_IF_FAILED(a) {if (FAILED(a)) return nullptr;}

namespace Skuld
{
	namespace Render3D
	{
		std::tuple<DXGI_FORMAT, UINT> SelectDXGIFormat(ShaderInputLayoutAttriType mType, uint32_t mSize);
		DXGI_FORMAT PixelFormatToDXGIFormat(PixelFormat mFormat);
		UINT BufferBindFlagToD3D11BindFlag(BufferBindFlag mBind);
		UINT TextureBindFlagToD3D11BindFlag(TextureBindFlag mBind);
		D3D11_USAGE AccessFlagToUsage(AccessFlag mAccess);
	}
}