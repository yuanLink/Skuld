#include "D3D11Utility.h"

namespace Skuld
{
	namespace Render3D
	{
		std::tuple<DXGI_FORMAT, UINT> SelectDXGIFormat(ShaderInputLayoutAttriType mType, uint32_t mSize)
		{
			DXGI_FORMAT mRet1 = DXGI_FORMAT_UNKNOWN;
			UINT mRet2 = 0;

			switch (mType)
			{
			case Render3D::ShaderInputLayoutAttriType_int:
			case Render3D::ShaderInputLayoutAttriType_uint:
			case Render3D::ShaderInputLayoutAttriType_float:
				mRet2 = 4;
				break;
			case Render3D::ShaderInputLayoutAttriType_short:
			case Render3D::ShaderInputLayoutAttriType_ushort:
				mRet2 = 2;
				break;
			case Render3D::ShaderInputLayoutAttriType_byte:
			case Render3D::ShaderInputLayoutAttriType_sbyte:
				mRet2 = 1;
				break;
			default:
				break;
			}

			mRet2 *= mSize;

			if (mSize == 4)
			{
				switch (mType)
				{
				case ShaderInputLayoutAttriType_int: mRet1 = DXGI_FORMAT_R32G32B32A32_SINT; break;
				case ShaderInputLayoutAttriType_uint: mRet1 = DXGI_FORMAT_R32G32B32A32_UINT; break;
				case ShaderInputLayoutAttriType_float: mRet1 = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				case ShaderInputLayoutAttriType_short: mRet1 = DXGI_FORMAT_R16G16B16A16_SINT; break;
				case ShaderInputLayoutAttriType_ushort: mRet1 = DXGI_FORMAT_R16G16B16A16_UINT; break;
				case ShaderInputLayoutAttriType_sbyte: mRet1 = DXGI_FORMAT_R8G8B8A8_SINT; break;
				case ShaderInputLayoutAttriType_byte: mRet1 = DXGI_FORMAT_R8G8B8A8_UINT; break;
				default: break;
				}
			}
			else if (mSize == 3)
			{
				switch (mType)
				{
				case ShaderInputLayoutAttriType_int: mRet1 = DXGI_FORMAT_R32G32B32_SINT; break;
				case ShaderInputLayoutAttriType_uint: mRet1 = DXGI_FORMAT_R32G32B32_UINT; break;
				case ShaderInputLayoutAttriType_float: mRet1 = DXGI_FORMAT_R32G32B32_FLOAT; break;
				default: break;
				}
			}
			else if (mSize == 2)
			{
				switch (mType)
				{
				case ShaderInputLayoutAttriType_int: mRet1 = DXGI_FORMAT_R32G32_SINT; break;
				case ShaderInputLayoutAttriType_uint: mRet1 = DXGI_FORMAT_R32G32_UINT; break;
				case ShaderInputLayoutAttriType_float: mRet1 = DXGI_FORMAT_R32G32_FLOAT; break;
				case ShaderInputLayoutAttriType_short: mRet1 = DXGI_FORMAT_R16G16_SINT; break;
				case ShaderInputLayoutAttriType_ushort: mRet1 = DXGI_FORMAT_R16G16_UINT; break;
				case ShaderInputLayoutAttriType_sbyte: mRet1 = DXGI_FORMAT_R8G8_SINT; break;
				case ShaderInputLayoutAttriType_byte: mRet1 = DXGI_FORMAT_R8G8_UINT; break;
				default: break;
				}
			}
			else if (mSize == 4)
			{
				switch (mType)
				{
				case ShaderInputLayoutAttriType_int: mRet1 = DXGI_FORMAT_R32_SINT; break;
				case ShaderInputLayoutAttriType_uint: mRet1 = DXGI_FORMAT_R32_UINT; break;
				case ShaderInputLayoutAttriType_float: mRet1 = DXGI_FORMAT_R32_FLOAT; break;
				case ShaderInputLayoutAttriType_short: mRet1 = DXGI_FORMAT_R16_SINT; break;
				case ShaderInputLayoutAttriType_ushort: mRet1 = DXGI_FORMAT_R16_UINT; break;
				case ShaderInputLayoutAttriType_sbyte: mRet1 = DXGI_FORMAT_R8_SINT; break;
				case ShaderInputLayoutAttriType_byte: mRet1 = DXGI_FORMAT_R8_UINT; break;
				default: break;
				}
			}

			return std::make_tuple(mRet1, mRet2);
		}

		DXGI_FORMAT PixelFormatToDXGIFormat(PixelFormat mFormat)
		{
			switch (mFormat)
			{
			case PixelFormat_RGBA_8888:return DXGI_FORMAT_R8G8B8A8_UNORM;
			case PixelFormat_BGRA_8888:return DXGI_FORMAT_B8G8R8A8_UNORM;
			case PixelFormat_RGB_565:return DXGI_FORMAT_B5G6R5_UNORM;
			case PixelFormat_ARGB_1555:return DXGI_FORMAT_B5G5R5A1_UNORM;
			default:return DXGI_FORMAT_UNKNOWN;
			}
		}
	}
}