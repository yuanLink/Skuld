#include "Compiler.h"
#include <functional>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		std::shared_ptr<Compiler> CreateDXBCCompiler();
		std::shared_ptr<Compiler> CreateDXILCompiler();
		std::shared_ptr<Compiler> CreateGLSLCompiler();
		std::shared_ptr<Compiler> CreateSPIRVCompiler();
		std::shared_ptr<Compiler> CreateMetalCompiler();
	}
}

namespace Skuld
{
	namespace Extra
	{
		namespace {
			class CompilerRegister {
			public:
				std::vector<std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()>> mObjects;
				void Register(std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()> func)  {
					mObjects.push_back(func);
				}

				CompilerRegister() {
					Register(ShaderCrossCompiler::CreateDXBCCompiler);
					Register(ShaderCrossCompiler::CreateSPIRVCompiler);
					Register(ShaderCrossCompiler::CreateGLSLCompiler);
					Register(ShaderCrossCompiler::CreateMetalCompiler);
					Register(ShaderCrossCompiler::CreateDXILCompiler);
				}
			};

			static CompilerRegister mRegister;

			class SCC_DataImpl : public SCC_Data
			{
			protected:
				virtual ~SCC_DataImpl() {}
			public:
				std::string mErrOrWarning;
				std::vector<uint8_t> mData;

				virtual const uint8_t *Data() const { return mData.data(); }
				virtual size_t DataSize() const { return mData.size(); }
				virtual const char* ErrorOrWarning() const { return mErrOrWarning.c_str(); }
			};
		}

		SCC_Data* CompileHLSL(const char* mHLSL, ShaderBinaryFormat mFormat,
			Render3D::ShaderType mProfile, const char* mMain)
		{
			for (std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()>& mCompilerFactory : mRegister.mObjects)
			{
				std::shared_ptr<ShaderCrossCompiler::Compiler> mCompiler = mCompilerFactory();
				if (mCompiler->Format() == mFormat)
				{
					try
					{
						Ptr<SCC_DataImpl> mRet(new SCC_DataImpl());
						bool mStatus = mCompiler->Compile(mHLSL, mRet->mData, mProfile, mMain);
						if (!mStatus) {
							mRet->mErrOrWarning = mCompiler->mWarning;
						}
						return mRet.Detach();
					}
					catch (Exception& e)
					{
						Ptr<SCC_DataImpl> mRet(new SCC_DataImpl());
						mRet->mErrOrWarning = e.Message().GetStr();
						return mRet.Detach();
					}
				}
			}
			return nullptr;
		}
	}
}

#ifdef SKULD_BUILD_DYNAMIC
extern "C"
SKULD_EXTRA_EXPORT Skuld::Extra::SCC_Data* Skuld_CompileHLSL(const char* mHLSL, Skuld::Extra::ShaderBinaryFormat mFormat,
	Skuld::Render3D::ShaderType mProfile, const char* mMain) {
	return Skuld::Extra::CompileHLSL(mHLSL, mFormat, mProfile, mMain);
}

#endif