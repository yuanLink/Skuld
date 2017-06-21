#include "Compiler.h"

#ifdef WIN32

#include <d3dcompiler.h>
#include <atlbase.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		class DXBCCompiler : public Compiler
		{
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_DXBC; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst);
		};

		bool DXBCCompiler::Compile(const char* mSrc, std::vector<uint8_t>& mDst)
		{
			CComPtr<ID3D10Blob> mOutput, mErr;

			HRESULT hr = D3DCompile(mSrc, -1, "scc.hlsl", nullptr,
				nullptr, mEntry.c_str(), mProfile.c_str(), 0, 0, &mOutput, &mErr);

			if (!FAILED(hr))
			{
				mDst.resize(mOutput->GetBufferSize());
				memcpy(&mDst[0], mOutput->GetBufferPointer(), mDst.size());

				if (mErr)
				{
					this->mWarning = (char*)mErr->GetBufferPointer();
					return false;
				}

				return true;
			}

			if (mErr)
				throw Exception((char*)mErr->GetBufferPointer());
			
			return false;
		}
	}
}

#else

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		class DXBCCompiler : public Compiler
		{
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_DXBC; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst) { throw Exception("²»Ö§³ÖDXBC"); }
		};
	}
}

#endif

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		std::shared_ptr<Compiler> CreateeDXBCCompiler() {
			return std::make_shared<DXBCCompiler>();
		}
	}
}