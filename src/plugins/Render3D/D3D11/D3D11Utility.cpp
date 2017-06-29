#include "D3D11Utility.h"
#include <Skuld/Exception.h>

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

		UINT BufferBindFlagToD3D11BindFlag(BufferBindFlag mBind)
		{
			UINT mRet = 0;
			if (mBind & Skuld::Render3D::BufferBind_VertexBuffer)
				mRet |= D3D11_BIND_VERTEX_BUFFER;
			if (mBind & Skuld::Render3D::BufferBind_IndexBuffer)
				mRet |= D3D11_BIND_INDEX_BUFFER;
			if (mBind & Skuld::Render3D::BufferBind_ConstantBuffer)
				mRet |= D3D11_BIND_CONSTANT_BUFFER;
			if (mBind & Skuld::Render3D::BufferBind_ShaderResource)
				mRet |= D3D11_BIND_SHADER_RESOURCE;

			return mRet;
		}

		UINT TextureBindFlagToD3D11BindFlag(TextureBindFlag mBind)
		{
			UINT mRet = 0;

			if (mBind & Skuld::Render3D::TextureBind_ShaderResource)
				mRet |= D3D11_BIND_SHADER_RESOURCE;

			return mRet;
		}

		D3D11_USAGE AccessFlagToUsage(AccessFlag mAccess)
		{
			if (mAccess == Access_GPURead) return D3D11_USAGE_IMMUTABLE;
			else if ((mAccess & Access_CPURead) == 0 && (mAccess & Access_GPUWrite) == 0)
				return D3D11_USAGE_DYNAMIC;
			else if ((mAccess & Access_CPURead) == 0 && (mAccess & Access_CPUWrite) == 0)
				return D3D11_USAGE_DEFAULT;
			else return D3D11_USAGE_STAGING;
		}
	}
}